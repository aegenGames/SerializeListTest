using namespace std;

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class ListNode
{
	public:
		ListNode * Prev = nullptr;
		ListNode * Next = nullptr;
		ListNode * Rand = nullptr;
		string Data;

		ListNode() {
		}

		ListNode(const string & data) {
			this->Data = data;
		}

		~ListNode()
		{
		}

		void setRand(ListNode * node) {
			this->Rand = node;
		}

		bool compare(const ListNode & node) {
			if (this->Data.compare(node.Data))
				return false;
			if (this->Rand->Data.compare(node.Rand->Data))
				return false;

			return true;
		}
};

class ListRand
{
	public:
		ListNode * Head = nullptr;
		ListNode * Tail = nullptr;
		int Count = 0;

		ListRand() {
		}

		ListRand(const initializer_list<string>& data) {
			for (auto str : data) {
				push_back(str);
			}
		}

		~ListRand() {
			this->clear();
		}

		void Serialize(ostream & stream) {

			stream << Count << endl;
			int index = 0;
			for (auto node = this->Head; node != nullptr; node = node->Next)
			{
				stream << node->Data << endl;
				* reinterpret_cast <int*> (&(node->Prev)) = index;
				++index;
			}

			for (auto node = this->Head; node != nullptr; node = node->Next)
			{
				stream << reinterpret_cast <int> (node->Rand->Prev) << endl;
			}
		}

		void Deserialize(istream & stream) {

			clear();
			int countElements;
			stream >> countElements;

			vector<ListNode*> randomPointers;
			randomPointers.reserve(Count);

			string tmpData;
			for (int i = 0; i < countElements; ++i)
			{
				stream >> tmpData;
				push_back(tmpData);
				randomPointers.push_back(Tail);
			}

			int index;
			for (auto node = this->Head; node != nullptr; node = node->Next)
			{
				stream >> index;
				node->Rand = randomPointers[index];
			}
		}

		void push_back(ListNode * node) {

			if (Head == nullptr) {
				Head = node;
			}
			else {
				Tail->Next = node;
				node->Prev = Tail;
			}
			Tail = node;
			++Count;
		}

		void push_back(const string & data) {
			ListNode * node = new ListNode(data);
			push_back(node);
		}

		ListNode * GetRandomElement() {

			int randomNumber;
			int counter = 0;

			randomNumber = rand() % this->Count;
			for (auto node = Head; node != nullptr; node = node->Next) {
				++counter;
				if (counter > randomNumber)
					return node;
			}
		}

		void FillFIeldRandom() {

			for (auto node = Head; node != nullptr; node = node->Next)
			{
				node->Rand = GetRandomElement();
			}
		}

		void clear() {

			if (Count == 0)
				return;
			Count = 0;
			while (this->Head)
			{
				Tail = Head->Next;
				delete Head;
				Head = Tail;
			}
		}

		bool compare(const ListRand & listCmpr) {
			if (this->Count != listCmpr.Count)
				return false;
			ListNode * nodeCmpr = listCmpr.Head;

			for (auto node = this->Head; node != nullptr; node = node->Next)
			{
				if (!node->compare(*nodeCmpr)) {
					return false;
				}
				nodeCmpr = nodeCmpr->Next;
			}

			return true;
		}
};

int main()
{
	ListRand serializeList;
	string str;
	int countElements = 5000;
	for (int i = 0; i < countElements; ++i) {
		str = to_string(i);
		serializeList.push_back(str);
	}
	
	serializeList.FillFIeldRandom();

	ofstream out;
	out.open("test.txt");
	serializeList.Serialize(out);
	out.close();

	ListRand deserializeList;
	ifstream in;
	in.open("test.txt");
	deserializeList.Deserialize(in);
	in.close();

	if (serializeList.compare(deserializeList)) {
		cout << "success" << endl;
	}
	else {
		cout << "failure" << endl;
	}
}
