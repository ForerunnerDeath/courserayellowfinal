#include "node.h"

bool EmptyNode :: Evaluate(const Date& d, const string& ev) const {
	return true;
}

bool DateComparisonNode :: Evaluate(const Date& d, const string& ev) const {
	if (cmp_ == Comparison::Less) {
		return d < data_;
	}
	else if (cmp_ == Comparison::LessOrEqual) {
		return d <= data_;
	}
	else if (cmp_ == Comparison::Greater) {
		return d > data_;
	}
	else if (cmp_ == Comparison::GreaterOrEqual) {
		return d >= data_;
	}
	else if (cmp_ == Comparison::Equal) {
		return d == data_;
	}
	else if (cmp_ == Comparison::NotEqual) {
		return d != data_;
	} else {
		return false;
	}
}

bool EventComparisonNode :: Evaluate(const Date& d, const string& ev) const {
	if (cmp_ == Comparison::Less) {
		return ev < event_;
	}
	else if(cmp_ == Comparison::LessOrEqual) {
		return ev <= event_;
	}
	else if (cmp_ == Comparison::Greater) {
		return ev > event_;
	}
	else if (cmp_ == Comparison::GreaterOrEqual) {
		return ev >= event_;
	}
	else if (cmp_ == Comparison::Equal) {
		return ev == event_;
	}
	else if (cmp_ == Comparison::NotEqual) {
		return ev != event_;
	} else {
		return false;
	}
}

bool LogicalOperationNode :: Evaluate(const Date& d, const string& ev) const {
	if (log_operation_ == LogicalOperation::And) {
		return left_->Evaluate(d, ev) && right_->Evaluate(d, ev);
	}
	else {
		return left_->Evaluate(d, ev) || right_->Evaluate(d, ev);
	}
}




