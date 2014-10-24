#include "query.h"
#include "TextQuery.h"
#include <string>
#include <set>
#include <iostream>

using std::set;
using std::string;
using std::cout; using std::cerr;
using std::endl;

int main(int, char **argv)
{
	// gets file to read and builds map to support queries
	TextQuery file = build_textfile("C:\\Users\\izhaolei\\Desktop\\MS_files\\15\\data\\main5_in");

	// iterate with the user: prompt for a word to find and print results
	do {
		string sought1, sought2;
		// stop if hit eof on input or a "q" is entered
		if (!get_words(sought1, sought2)) break;

		// find all the occurrences of the requested string
		Query andq = Query(sought1) & Query(sought2);
		set<TextQuery::line_no> locs = andq.eval(file);
		cout << "\nExecuted query: " << andq << endl;
		// report matches
		print_results(locs, file);

		locs = Query(sought1).eval(file);
		cout << "\nExecuted query: " << Query(sought1) << endl;
		// report matches
		print_results(locs, file);

		locs = Query(sought2).eval(file);
		cout << "\nExecuted query: " << Query(sought2) << endl;
		// report matches
		print_results(locs, file);
	} while (true);
	return 0;
}
