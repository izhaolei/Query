#include "query.h"
#include "TextQuery.h"
#include <set>
#include <algorithm>
#include <iostream>

using namespace std;

set<TextQuery::line_no> NotQuery::eval(const TextQuery &file)const
{
	set<line_no> has_val = query.eval(file);//this call wordquery.eval()
	set<line_no> ret_lines;
	for (line_no no = 0; no != has_val.size();++no)
	{
		if (has_val.find(no) == has_val.end())
			ret_lines.insert(no);
	}
	return ret_lines;
}

set<TextQuery::line_no>
AndQuery::eval(const TextQuery& file) const
{
	// virtual calls through the Query handle to get result sets for the operands
	set<line_no> left = lhs.eval(file),
		right = rhs.eval(file);

	set<line_no> ret_lines;  // destination to hold results 

	// writes intersection of two ranges to a destination iterator
	// destination iterator in this call adds elements to ret
	set_intersection(left.begin(), left.end(),
		right.begin(), right.end(),
		inserter(ret_lines, ret_lines.begin()));
	return ret_lines;
}

// returns union of its operands' result sets
set<TextQuery::line_no>
OrQuery::eval(const TextQuery& file) const
{
	// virtual calls through the Query handle to get result sets for the operands
	set<line_no> left = lhs.eval(file),
		right = rhs.eval(file);

	// destination to hold results, start by copying lines from left
	set<line_no> ret_lines(left);

	// inserts the lines from right that aren't already in ret_lines
	ret_lines.insert(right.begin(), right.end());

	return ret_lines;
}
