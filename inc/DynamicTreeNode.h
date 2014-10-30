#ifndef DYNAMIC_TREENODE_H
#define DYNAMIC_TREENODE_H


#include <map>
#include <vector>
#include <iostream>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>




class Node{
	public:
		//virtual std::string path() = 0;
		//virtual Node* get(const std::string& name){
		virtual Node* find(const std::string& name){
			if(!name.empty() && _map.find(name) != _map.end()) return dynamic_cast<Node*>(_map.find(name)->second);
			else throw std::runtime_error("DynamicNode: "+name+" is not created.\n");
		}
		void regist(const std::string& name, Node* node){
			if(!name.empty() && _map.find(name) == _map.end()) _map.insert(std::pair<std::string, Node*>(name, node));
			else throw std::runtime_error(name + " is already existed.\n");
		}

	private:
		typedef std::map<std::string, Node*> NodeMap;
		NodeMap _map;

};


template<class O> class DynamicTree;

template<class O>
class DynamicObj: public Node{
	friend class DynamicTree<O>;

	public: O* get(){ 
			if(m_ref) return m_ref; 
			else throw std::runtime_error("DynamicObj is not registed.\n");
		}
		void regist(const std::string& name, O* obj){
			if(name.empty()) throw std::runtime_error("regist object error: no name for object");
			m_ref = obj; m_name = name;
		}
		virtual Node* find(std::string name){
			if(name==m_name) return this;
			else throw std::runtime_error("DynamicObj "+name+" is not created.\n");
		}
		//virtual std::string path() // ?? need path parent
	private:
		DynamicObj(std::string name, O* obj){
			this->regist(name, obj);
		}
		DynamicObj(){};
	private:
		O* m_ref;
		std::string m_name;
};

template<class O>
class DynamicTree: public Node{
	public:
		DynamicTree(){
			m_parent = 0;
			m_root   = this;
			m_path   = "/";
		}

		virtual Node* find(const std::string& name){
			vector<std::string> dividedName;
			vector<std::string>::iterator itName;
			boost::split(dividedName, name, boost::is_any_of("/"), boost::token_compress_on);
			Node* current = (0==name.find("/"))?m_root:this;
			for(itName=dividedName.begin(); itName!=dividedName.end(); itName++){
				if(0==(*itName).size()) continue;
				if(dynamic_cast<DynamicTree<O>*>(current)) current = current->Node::find(*itName);
			        else current = current->find(*itName);	
				// how to throw out error? path
			}
			return current;
		}

		virtual Node* branch(const std::string& name){
			if(name.empty()) throw std::runtime_error("make branch error: no name for branch");
			DynamicTree<O>* br = new DynamicTree<O>(name, this);
			this->regist(name, br);
			// if error delete br!
			return br;
		}

		virtual void registObj(const std::string& name, O* o){
			if(name.empty()) throw std::runtime_error("make branch error: no name for branch");
			DynamicObj<O>* obj = new DynamicObj<O>(name, o);
			this->regist(name, obj);
		}

		std::string     path(){return m_path;}
		DynamicTree<O>* root(){return m_root;}
	private:
		DynamicTree(std::string name, DynamicTree<O>* parent){
			m_parent = parent;
			m_root   = parent->root();
			m_path   = m_parent->path()+(m_parent->path()=="/"?name:"/"+name);
		}

	private:
		std::string      m_path;
		DynamicTree<O>*  m_root;
		DynamicTree<O>*  m_parent;
};


#endif
