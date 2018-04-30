//ANGELA VITALETTI	cs435 mp
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <vector>
using namespace std;

//Global Variables
int fm; //Used to represent the slots in the table

//Class for Hash Table Entry
class HashEntry_4417{
private:
	int index; //Index of the first character in the Hash Array
	string ref = " "; //Reference to a location in Array A
	string word; //The letter in a spot
public:
	HashEntry_4417() { index = 0; word = "_"; }
	HashEntry_4417(int i, string r){ index = i; word = r; }
	void initEntry(int idx) { index = idx; word = " "; }
	void setIndex (int n) { index = n; }
	void setRef (int n) {ref = to_string(n); }
	void setRef (string n) { ref = n; }
	int getIndex () { return index; }
	string getRef () { return ref; }
	string getVal() { return word; }
};

//Class for Hash Table, T
class HashTable_{
private:
	int collisions; //A nice way to keep track of the collision count
public:
	vector<HashEntry_4417> table; //Used to represent the table in memory
	HashTable_() {collisions = 0;}
	void createTable(int n){ 
		table.resize(n); 
		for (int i = 0; i < n; i++){
			table[i].initEntry(i);
		}
	}
	string getTableVal(int idx) {return table[idx].getVal();}
	void insert(HashEntry_4417 e) {table[e.getIndex()] = e;}
	void incC() {collisions++;}
	void resizeT(int n) { 
		int prevSize = table.size();
		table.resize(n);
		for (int i = prevSize; i < n; i++)
			table[i].initEntry(i);
	}
	int getTableSize() {return table.size(); }
	int getCollisons() { return collisions; }
	
};

//Class for the Lexicon, L
class lexicon_{
public:
	HashTable_ T; //Hash Table T
	string A; //A, technically an Array of chars. Additionally, a variable length array is not permitted in c++
	string defaultA; //Used to maintain a default A, which will be space chars of a certain length
};

//Function Prototypes
int HashBatch(lexicon_* L, ifstream &file);
int HashSearch(lexicon_* L, string w);
void HashCreate(lexicon_* L, int m);
void HashInsert(lexicon_* L, string w);
void HashDelete(lexicon_* L, string w);
void HashPrint(lexicon_* L);
bool HashEmpty(lexicon_ L); 
bool HashFull(lexicon_ L);


int main(int argc, char* argv[]){
	lexicon_ L;
	
	//Read in the arguments, check for errors
	if (argc <= 0)
		cout << "Something went wrong!\n";
	else if (argc == 2){ //Everything seems okay, let's continue the program
		ifstream file;
		file.open(argv[1]);
		HashBatch(&L, file);
		file.close();
	}
	else //Incorrect input format
		cout << "Incorrect format: Expected format is ./filename textfile.txt\n";
	return(0);
}

int HashBatch(lexicon_* L, ifstream &file){
	bool lexiconCreated = false; //Used to check if a lexicon was created. If it wasn't we can't do certain operations
	if (!file){ //We cannot open the file
		cout << "Unable to open file!\n";
		return(1);
	}
	else { //Let's read those lines of the file
		string line, command = "";
		int c = 0;
		while (file >> line){
			if (c == 0) { //Let's figure out the command to preform
				command = line;
				c++; //This makes me laugh
				if (command == "13" && lexiconCreated){ //We're printing, so the next value will be a command again
					c = 0;
					HashPrint(L);
				}
			}
			else { //We're given something to input, search, etc.
				c = 0;
				if (command == to_string(10) && lexiconCreated) { //Insert operation
					HashInsert(L, line);
				}
				else if (command == to_string(11) && lexiconCreated) { //Deletion Operation
					HashDelete(L, line);
				}
				else if (command == to_string(12) && lexiconCreated) { //Search Operation
					int slot = HashSearch(L, line);
					if (slot != -1)
						cout << line << "	found at slot " << slot << endl;
					else
						cout << line << "	not found\n";
				}
				else if (command == to_string(14) && !lexiconCreated){ //Create Operation
					lexiconCreated = true;
					HashCreate(L, stoi(line)); 
				}
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Function: HashCreate
// Description: Initializes (or reinitializes) the HashTable/Array A size and values
// Parameters: Reference to Lexicon L (the lexicon), int m (size of HashTable)
// Return Values: None
///////////////////////////////////////////////////////////////////////////////////////////////
void HashCreate(lexicon_* L, int m){
	int newSize = 15 * m;
	L->T.createTable(m); //Create an initialize the hash table
	L->A.resize(newSize, ' '); //Set the size of the array of characters
	L->defaultA = L->A; //Default string
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Function: HashInsert
// Description: Insert an entry into the Hash Table at a given index with a given "value"
// Parameters: Reference to Lexicon L (the lexicon), string w (the word)
// Return Values: None
///////////////////////////////////////////////////////////////////////////////////////////////
void HashInsert(lexicon_* L, string w){
	int pIndex = 0; //Set the probing index to 0
	int ASCII_VAL = 0;
	int h, h1;
	
	//First we need to find h'(k)
	for (int i = 0; i < w.length(); i++) {
		ASCII_VAL += (int)w[i];
	}
	h1 = ASCII_VAL % L->T.getTableSize();
	//Now we find h(k)
	h = (h1 + (pIndex * pIndex)) % L->T.getTableSize();
	
	//Check to see if that index is already taken
	if (L->T.table[h].getVal() != " ") { //There has been a collision
		L->T.incC(); //Increment collisions
		//Now we need to see if we can find another slot to put it in
		bool noneLeft = true;
		while (pIndex < L->T.getTableSize()){
			pIndex++; //Increment probing index
			h = (h1 + (pIndex * pIndex)) % L->T.getTableSize(); //Get the new index
			if (L->T.table[h].getVal() == " "){ //This index is free
				noneLeft = false;
				break;
			}
			else { //Still a collision
				L->T.incC(); //increment collisions
			}
		}
		if (noneLeft){ //We couldn't insert the value
			cout << "Unable to insert " << w << ", probing index limit reached\n";
			return;
		}
	}
	
	w = w + '\0'; //Add the null terminator CHARACTER, not two characters, to w
	//Find next available in A
	int spot;
	for (int i = 0; i < L->A.length(); i++){
		if (L->A[i] == ' ') {
			spot = i;
			break;
		}
	}
	
	//Everything is fine, insert as normal
	HashEntry_4417 nE(h, w);
	nE.setRef(spot);
	L->T.insert(nE);
	
	//Put in the new word into A
	int j = 0; //Separate iterator for w
	if (w.length() <= L->A.length()) { //It will fit!
		for (int i = spot; i < L->A.length(); i++){
			L->A[i] = w[j];
			j++;
			if (j == w.length())
				break;
		}
	}
	else { //We don't have enough available space in A, let's try and make some
		L->A.resize(L->A.length() + w.length(), ' '); //Increase the size of A by the size of the word
		for (int i = spot; i < L->A.length(); i++){
			L->A[i] = w[j];
			j++;
			if (j == w.length())
				break;
		}
	}
}	

///////////////////////////////////////////////////////////////////////////////////////////////
// Function: HashDelete
// Description: Delete an entry in hash table T 
// Parameters: Reference to Lexicon L (the lexicon), string w (the word)
// Return Values: None
///////////////////////////////////////////////////////////////////////////////////////////////
void HashDelete(lexicon_* L, string w){
	int idx = HashSearch(L, w); //Find where the element is
	if (idx != -1){
		int j = 0;
		int refIdx = stoi(L->T.table[idx].getRef());
		for (int i = refIdx; i < L->A.length(); i++){
			L->A[i] = '*';
			j++;
			if (j == w.length())
				break;
		}
		L->T.table[idx].setRef(" ");
		cout << w << "	deleted from slot " << idx << endl;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Function: HashSearch
// Description: Search the Hash Table for a word
// Parameters: Reference to Lexicon L (the lexicon), string w (the word)
// Return Values: The index of the element in the hash table or -1 if not found
///////////////////////////////////////////////////////////////////////////////////////////////
int HashSearch(lexicon_* L, string w){
	w = w + '\0';
	for (int i = 0; i < L->T.getTableSize(); i++){
		if (L->T.table[i].getVal() == w) {
			return i; //Found it!
		}
	}
	return -1; //Not found
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Function: HashEmpty
// Description: Checks to see if a HashTable is empty
// Parameters: Lexicon L (the lexicon)
// Return Values: True (is empty), False (not empty)
///////////////////////////////////////////////////////////////////////////////////////////////
bool HashEmpty(lexicon_ L){
	if (L.T.getTableSize() != 0){
		for (int i = 0; i < L.T.getTableSize(); i++){
			if (L.T.getTableVal(i) != " " || L.T.getTableVal(i) != "_") return false; //Something was in the table
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Function: HashFull
// Description: Determine if the Hash Table is full
// Parameters: Reference to Lexicon L (the lexicon)
// Return Values: True (full), False (empty)
///////////////////////////////////////////////////////////////////////////////////////////////
bool HashFull(lexicon_ L){
	for (int i = 0; i < L.T.getTableSize(); i++){
		if (L.T.getTableVal(i) == " " || L.T.getTableVal(i) == "_"){
			return false; //There is still space in the table
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: HashPrint
// Description: Print the contents of the hash table (indexes of the first letter of a word in array A) and array A
// Parameters: Reference to Lexicon L (the lexicon)
// Return Values: None
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HashPrint(lexicon_* L){
	cout <<	"\n  T			A: ";
	int spot = L->A.length() - 1;
	for (int i = 0; i < L->A.length(); i++){
		if (L->A[i] == ' ') {
			spot = i;
			break;
		}
	}
	for (int i = 0; i < L->A.length(); i++){
		if (L->A[i] == '\0')
			cout << '\\';
		else if (i == spot){ //On the last letter
			cout << L->A[i] << endl;
			break;
		}
		else 
			cout << L->A[i];
	}
	for (int i = 0; i < L->T.getTableSize(); i++){
		cout << i << ": " << L->T.table[i].getRef() << endl;
	}
	cout << endl;
}