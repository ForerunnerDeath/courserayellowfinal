#pragma once

#include "date.h"
#include <string>
#include <memory>

enum class Comparison{
Less,
LessOrEqual,
Greater,
GreaterOrEqual,
Equal,
NotEqual
};

enum class LogicalOperation {
Or,
And
};

class Node {
public:
	virtual bool Evaluate(const Date& d, const string& ev) const = 0;
};

class EmptyNode : public Node {
public:
	bool Evaluate(const Date& d, const string& ev) const override;
};

class DateComparisonNode : public Node {
public:
	DateComparisonNode(Comparison comparison, const Date& d) : cmp_(comparison), data_(d) {}
	bool Evaluate(const Date& d, const string& ev) const override;
private:
	Comparison cmp_;
	const Date data_;
};

class EventComparisonNode : public Node {
public:
	EventComparisonNode(Comparison comparison, const string& event) : cmp_(comparison), event_(event) {}
	bool Evaluate(const Date& d, const string& ev) const override;
private:
	Comparison cmp_;
	const string event_;
};

class LogicalOperationNode : public Node {
public:
	LogicalOperationNode(LogicalOperation log, shared_ptr<Node> left, shared_ptr<Node> right) : log_operation_(log), left_(left), right_(right) {}
	bool Evaluate(const Date& d, const string& ev) const override;
private:
	LogicalOperation log_operation_;
	shared_ptr<Node> left_;
	shared_ptr<Node> right_;
};







