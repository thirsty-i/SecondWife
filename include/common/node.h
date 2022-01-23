#pragma once

struct forward_node
{
	forward_node* next;
};

struct bidirectional_node
{
	forward_node* prev;
	forward_node* next;
};