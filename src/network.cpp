#include <algorithm>
#include "network.h"
#include "random.h"
using namespace std;


void Network::resize(const size_t& new_size) {
	values.clear();
	values.resize(new_size);
	RNG.normal(values,0,1); 
}

bool Network::add_link(const size_t& node_1, const size_t& node_2) {
	if ((node_1 >= values.size()) or (node_2 >= values.size()) or (node_1 == node_2)) {
		return false;
	}
	
	for (multimap<size_t, size_t>::iterator i = links.begin(); i != links.end(); ++i) {
		if (i->first == node_1 and i->second == node_2) {
			return false;
		}
	}
	
	pair<size_t, size_t> link_one = make_pair(node_1, node_2);
	pair<size_t, size_t> link_two = make_pair(node_2, node_1);
	links.insert(link_one);
	links.insert(link_two);
	return true;
}

	
size_t Network::random_connect(const double& mean) {
	links.erase(links.begin(), links.end());
	size_t count_up(0);
	
	for (size_t i(0); i < values.size(); ++i) {
		int nb_links(RNG.poisson(mean));
		//int nb_links(RNG.poisson(mean)-degree(values[i]));
		//if (nb_links > 0) {
		vector<int> nodes_links(nb_links);
		int lower(0);
		int upper(values.size());
		RNG.uniform_int(nodes_links, lower, upper);
		for (int j(0); j < nb_links; ++j) {
			if (values[i] != nodes_links[j]) {
				++count_up; 
			} 
			add_link(values[i], nodes_links[j]);
		}
	//}
	}
	return count_up;
}
	
size_t Network::set_values(const vector<double>& new_vector) {
	size_t nodes_reset(0);
	if (new_vector.size() > values.size()) {
		for (size_t i(0); i < values.size(); ++i) {
			values[i]=new_vector[i];
			++nodes_reset;
		}
	} else {
		for (size_t i(0); i < new_vector.size(); ++i) {
			values[i]=new_vector[i];
			++nodes_reset;
		}
	}
	return nodes_reset;
}
	
size_t Network::size() const {
	return values.size();
}

size_t Network::degree(const size_t &_n) const {
	return links.count(_n);
}
	
double Network::value(const size_t &_n) const {
	size_t row(_n);
	return values[row];
}
	
vector<double> Network::sorted_values() const {
	vector<double> values_descending = values;
	std::sort(values_descending.begin(), values_descending.end());
	std::reverse(values_descending.begin(), values_descending.end());
	return values_descending;
}
	
vector<size_t> Network::neighbors(const size_t& _n) const {
	vector<size_t> neighbors_nodes;
	for (multimap<size_t, size_t>::const_iterator i = links.begin(); i != links.end(); ++i) {
		if (i->first == _n) {
			neighbors_nodes.push_back(i->second);
		}
	}
	return neighbors_nodes;
}
