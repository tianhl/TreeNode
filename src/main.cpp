#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
using namespace std;

#include "DynamicTreeNode.h"
#include "Pulse.h"
#include "Event.h"

int main(int argc, char *argv[]) {

	typedef DynamicTree<DataObject> DataStore;
	typedef DynamicObj<DataObject> Data;

	DataStore* datastore = new DataStore();
	std::cout << "root datastore: " << datastore->path() << std::endl;
	std::cout << dynamic_cast<DataStore*>(datastore->find("/")) << std::endl;
	std::cout << "new branch " << datastore->branch("pulse") << std::endl;
	std::cout << dynamic_cast<DataStore*>(datastore->find("/pulse"))->path() << std::endl;
	dynamic_cast<DataStore*>(datastore->find("/pulse"))->branch("event");
	std::cout << dynamic_cast<DataStore*>(datastore->find("pulse/event"))->path() << std::endl;

	dynamic_cast<DataStore*>(datastore->find("/pulse"))->registObj("pulseHdr", new Pulse());
	std::cout << "======================" << std::endl;
	Data* data = dynamic_cast<Data*>(datastore->find("/pulse/pulseHdr"));
	std::cout << "obj: " << data << " data " << data->get() << " type "<<  typeid(data->get()).name() << std::endl;
	dynamic_cast<Pulse*>(data->get());

	std::cout << "======================" << std::endl;
	Event* evt = new Event();
	std::cout << "new event: " << evt << std::endl;
	dynamic_cast<DataStore*>(datastore->find("/pulse/event"))->registObj("evtHdr", evt);
	Node* node = datastore->find("/pulse/event/evtHdr");
	Data* event = dynamic_cast<Data*>(node);
	std::cout << "get event: " << node << " " << event->get() << std::endl;
	dynamic_cast<Event*>(event->get());


	return 1;
}
