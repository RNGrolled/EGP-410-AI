#pragma once

#include <Trackable.h>

const int BAD_NODE_ID = -1;
#define NODE_ID int

class Node:public Trackable
{
public:
	Node( const NODE_ID& id );
	Node();
	~Node();

	const NODE_ID& getId() const {return mId;};
	NODE_ID& getPrev()  { return mPrev; } const
	void setPrevious(NODE_ID prev) { mPrev = prev; }

private:
	const NODE_ID mId;
	NODE_ID mPrev;
};