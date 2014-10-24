#ifndef QUERY_H
#define QUERY_H

#include "TextQuery.h"
#include <string>
#include <set>
#include <iostream>
#include <fstream>

using namespace std;

class Query_base
{
	friend class Query;
protected:
	typedef TextQuery::line_no line_no;
	virtual ~Query_base(){}
private:
	virtual set<line_no> eval(const TextQuery &) const = 0;
	virtual ostream& display(ostream &os = cout) const = 0;
};

class Query
{
	friend Query operator~(const Query&);
	friend Query operator&(const Query&, const Query&);
	friend Query operator|(const Query&, const Query&);

public:
	Query(const string&);
	Query(const Query &que) :query(que.query), use(que.use){ ++*use; }
	Query& operator=(const Query&);
	set<TextQuery::line_no>
		eval(const TextQuery &que)const{ return query->eval(que); }
	ostream& display(ostream &os)const
	{
		return query->display(os);
	}

private:
	Query(Query_base *que) :query(que), use(new size_t(1)){}
	Query_base *query;
	size_t *use;
	void del_use()
	{
		if (--*use == 0) { delete query; delete use; }
	}
};


inline Query& Query::operator=(const Query &q)
{
	++*q.use;
	del_use();
	query = q.query;
	use = q.use;
	return *this;
}

inline std::ostream&
operator<<(std::ostream &os, const Query &q)
{
	return q.display(os);
}

class WordQuery :public Query_base
{
	friend class Query;

private:
	WordQuery(const string &s) :query_word(s){}
	set<line_no> eval(const TextQuery &t) const
	{
		return t.run_query(query_word);
	}
	ostream& display(ostream &os)const
	{
		return os << query_word;
	}
	string query_word;
};

inline Query::Query(const string &s) :query(new WordQuery(s)), use(new size_t(1)){}

class NotQuery : public Query_base {
	friend Query operator~(const Query &);
	NotQuery(Query q) : query(q) { }

	// concrete class: NotQuery defines all inherited pure virtual functions
	std::set<line_no> eval(const TextQuery&) const;
	std::ostream& display(std::ostream &os) const
	{
		return os << "~(" << query << ")";
	}
	const Query query;
};

class BinaryQuery : public Query_base {
protected:
	BinaryQuery(Query left, Query right, std::string op) :
		lhs(left), rhs(right), oper(op) { }

	// abstract class: BinaryQuery doesn't define eval 
	std::ostream& display(std::ostream &os) const
	{
		return os << "(" << lhs << " " << oper << " "
			<< rhs << ")";
	}

	const Query lhs, rhs;   // right- and left-hand operands
	const std::string oper; // name of the operator
};

class AndQuery : public BinaryQuery {
	friend Query operator&(const Query&, const Query&);
	AndQuery(Query left, Query right) :
		BinaryQuery(left, right, "&") { }

	// concrete class: AndQuery inherits display and defines remaining pure virtual
	std::set<line_no> eval(const TextQuery&) const;
};

class OrQuery : public BinaryQuery {
	friend Query operator|(const Query&, const Query&);
	OrQuery(Query left, Query right) :
		BinaryQuery(left, right, "|") { }

	// concrete class: OrQuery inherits display and defines remaining pure virtual
	std::set<line_no> eval(const TextQuery&) const;
};

inline Query operator&(const Query &lhs, const Query &rhs)
{
	return new AndQuery(lhs, rhs);
}

inline Query operator|(const Query &lhs, const Query &rhs)
{
	return new OrQuery(lhs, rhs);
}

inline Query operator~(const Query &oper)
{
	return new NotQuery(oper);
}

std::ifstream& open_file(std::ifstream&, const std::string&);
TextQuery build_textfile(const std::string&);
bool get_word(std::string&);
bool get_words(std::string&, std::string&);
void print_results(const std::set<TextQuery::line_no>&, const TextQuery&);
#endif