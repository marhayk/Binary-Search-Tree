#pragma once
#include <iostream>
#include <fstream>
#include<string>

using namespace std;
struct Node
{
	int data;
	Node *left;
	Node *right;
};





class BTree
{
private:
	int newitem;
	Node * root;
	fstream file;
	int count, index;
	int *nums;
	bool balancing = false;

	void recinorder(Node *temp)
	{
		if (temp == NULL)
		{
			return;
		}
		else
		{
			recinorder(temp->left);

			if (balancing)
			{
				nums[index] = temp->data;
				index++;
			}
			else
			{
				cout << temp->data << "   ";
			}
			recinorder(temp->right);
		}
	}

	void recpreorder(Node *temp)
	{
		if (temp == NULL)
		{
			return;
		}
		else
		{
			if (file.is_open())
			{
				file << temp->data << "   ";
			}
			else
			{
				cout << temp->data << "   ";
			}
			recpreorder(temp->left);
			recpreorder(temp->right);
		}
	}

	void recpostorder(Node *temp)
	{
		if (temp == NULL)
		{
			return;
		}
		else
		{
			recpostorder(temp->left);
			recpostorder(temp->right);
			delete temp;
		}
	}

	void recbalance(int low, int high)
	{
		if (high >= low)
		{
			int mid = (low + high) / 2;
			if (mid < index)
			{
				add(nums[mid]);
			}
			if (low != high)
			{
				recbalance(low, mid - 1);
				recbalance(mid + 1, high);
			}
		}

	}

public:
	BTree()
	{
		root = NULL;
		count = 0;
		index = 0;
	}

	~BTree()
	{
		clear();
		delete[] nums;
	}
	void add(int item)
	{
		Node *trav = root;
		Node *temp = new Node;
		temp->data = item;
		temp->left = NULL;
		temp->right = NULL;
		bool is_added = false;

		if (root == NULL)
		{
			root = new Node;
			root->data = item;
			root->left = NULL;
			root->right = NULL;
		}
		else
		{
			while (!is_added)
			{
				if (temp->data > trav->data)
				{
					if (trav->right == NULL)
					{
						trav->right = temp;
						temp = NULL;
						is_added = true;
					}
					else
					{
						trav = trav->right;
					}
				}
				else
				{
					if (trav->left == NULL)
					{
						trav->left = temp;
						temp = NULL;
						is_added = true;
					}
					else
					{
						trav = trav->left;
					}
				}
			}
		}
		count++;
	}

	void inorder()
	{
		index = 0;
		recinorder(root);
		cout << endl;
	}

	void balance()
	{
		int temp_count = count;
		nums = new int[count];
		balancing = true;
		inorder();
		clear();
		recbalance(0, temp_count - 1);
		balancing = false;
	}

	bool search(int item)
	{
		Node *trav = root;
		Node *search_node = new Node;
		search_node->data = item;
		search_node->left = NULL;
		search_node->right = NULL;

		while (trav != NULL)
		{
			if (search_node->data > trav->data)
			{
				trav = trav->right;
			}
			else if (search_node->data < trav->data)
			{
				trav = trav->left;
			}
			else
			{
				cout << "Found" << endl;
				return true;
			}
		}
		cout << "Not found" << endl;
		return false;
	}

	Node* found(int item, Node **&parentPtr)
	{
		Node *trav = root;
		parentPtr = &root;
		Node *search_node = new Node;
		search_node->data = item;
		search_node->left = NULL;
		search_node->right = NULL;

		while (trav != NULL)
		{
			if (search_node->data > trav->data)
			{
				parentPtr = &(trav->right);
				trav = trav->right;
			}
			else if (search_node->data < trav->data)
			{
				parentPtr = &(trav->left);
				trav = trav->left;
			}
			else
			{
				return trav;
			}
		}
		return NULL;
	}


	void clear()
	{
		recpostorder(root);
		root = NULL;
		count = 0;
	}

	void delete_item(int item)
	{
		Node **parentPtr;
		Node *temp = found(item, parentPtr);
		if (temp != NULL)
		{
			if (temp->right != NULL)
			{
				Node **trav = &(temp->right);
				while ((*trav)->left != NULL)
				{
					trav = &((*trav)->left);
				}
				temp->data = (*trav)->data;
				Node *temp2 = *trav;
				*trav = temp2->right;
				delete temp2;
			}
			else if (temp->left != NULL)
			{
				Node **trav = &(temp->left);
				while ((*trav)->right != NULL)
				{
					trav = &((*trav)->right);
				}
				temp->data = (*trav)->data;
				Node *temp2 = *trav;
				*trav = temp2->left;
				delete temp2;
			}
			else
			{
				delete *parentPtr;
				*parentPtr = NULL;
			}
		}
		else
		{
			cout << "No item to delete" << endl;
		}
		count--;
	}

	void save()
	{
		//balance();
		string file_name;
		cout << "Enter the file name to save the tree to->";
		cin >> file_name;
		file.open(file_name);
		recpreorder(root);
		file.close();
	}

	void load()
	{
		if (root != NULL)
		{
			clear();
		}
		string snum;
		int inum;
		string file_name;
		cout << "Enter the file name to load the tree from->";
		cin >> file_name;
		file.open(file_name);
		while (!file.eof())
		{
			file >> snum;
			inum = stoi(snum);
			add(inum);

		}
		file.close();
	}
};