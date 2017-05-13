//***********************************************************//
//***********************************************************//
//***  2016 Data Structure Project 3  Due 12/23 Fri. ********//
//***  use Huffman code to compress and extract file ********//
//***  implement in linux system                     ********//
//***********************************************************//
//***********************************************************//


#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<vector>
#include<utility>
#include<map>
#include<queue>
#include<cassert>

using namespace std;

//-----------------------------------------------------------//
//--- declaration of huffman tree node ----------------------//
//-----------------------------------------------------------//
class huffnode{
    public:

		// constructor & destructor
		huffnode(char c=0,int f=0,huffnode* l=NULL,huffnode* r=NULL) : ch(c),freq(f),left(l),right(r) {}
		~huffnode() {delete left; delete right;}

		// data members
		char ch;
        int freq;
		huffnode* left;
		huffnode* right;

		// cmp class for priority queue
		class cmp{
			public:
			bool operator() (huffnode* l , huffnode* r) {	return (l->freq) > (r->freq);	}
		};
};
//--- declaration of huffnode end ---------------------------//




//-----------------------------------------------------------//
//--- build the table of appearance freqency ----------------//
//-----------------------------------------------------------//
vector< pair<char,int> > build_table (string text)
{

	// use map to build the frequecy table
	map<char,int> map1;
	for (int i = 0; i < text.length(); ++i) {
        if (map1.find(text[i])==map1.end()) // not find
        map1.insert(make_pair(text[i],1));
        else
        map1.at(text[i]) ++;
    }

    // push the map into vector<pair <char,int> >
    vector< pair<char,int> > vec1;
    for( map<char,int>::iterator it = map1.begin() ; it!=map1.end() ; ++it )
    {
    	vec1.push_back(make_pair(it->first,it->second));
	}
	return vec1;
 }

//--- table end ---------------------------------------------//





//-----------------------------------------------------------//
//--- build the huffman tree --------------------------------//
//-----------------------------------------------------------//
huffnode* build_hufftree(vector< pair<char,int> > vec1){

	// use min_heap to store hufftree
	priority_queue<huffnode* ,vector<huffnode*> , huffnode::cmp> huff_heap;

	// construct leaf nodes and push nodes into heap
	for(vector< pair<char,int> >::iterator it = vec1.begin() ; it!=vec1.end() ; ++it)
	{
		huffnode* leaf = new huffnode(it->first,it->second);
		huff_heap.push(leaf);
	}


	// merge leaf nodes to hufftree until only root left in heap
	huffnode* root_ptr=NULL;
	while(huff_heap.size()>1)
	{

		huffnode* l;
		l = huff_heap.top();
		huff_heap.pop();

		huffnode* r;
		r = huff_heap.top();
		huff_heap.pop();

		root_ptr = new huffnode(-1,(l->freq+r->freq),l,r);
		huff_heap.push(root_ptr);
	}

	return root_ptr;
}
//--- build huffman tree end --------------------------------//



//-----------------------------------------------------------//
//--- print the huffman tree --------------------------------//
//-----------------------------------------------------------//

void print_tree(huffnode* root){

	deque< pair<huffnode *, int> > q;

    q.push_back(make_pair(root, 0));
    int curlevel = -1;
    while (!q.empty()) {
        huffnode *parent = q.front().first;
		if(parent==NULL)cout<<"fuck";
        int level = q.front().second;
        q.pop_front();
        if (curlevel != level) {
            curlevel = level;
            cout << "Level " << curlevel << endl;
        }
        if(parent==NULL)system("pause");
        cout << parent->freq << " " << parent->ch << endl;
        if (parent->left)
            q.push_back(make_pair(parent->left, level + 1));
        if (parent->right)
            q.push_back(make_pair(parent->right, level + 1));
    }

	return;
}
//--- print huffman tree end --------------------------------//


//-----------------------------------------------------------//
//--- build the code table ----------------------------------//
//-----------------------------------------------------------//

//pre-order travesal to hufftree
void preorder(huffnode* h,vector<bool>& cur_code,map< char,vector<bool> >& map1)
{
    if(h==NULL)return;
    //cur_node.
    huffnode* temp=h;
    if((temp->ch)>0)
        map1.insert(make_pair(temp->ch,cur_code));


    vector<bool> left_code(cur_code);
    left_code.push_back(0);
    preorder(h->left,left_code,map1);

    vector<bool> right_code(cur_code);
    right_code.push_back(1);
    preorder(h->right,right_code,map1);
    return;
}

map< char , vector<bool> > build_encode_table(huffnode* h)
{
	vector<bool> cur_code;
	map< char , vector<bool> > map1;
    preorder(h,cur_code,map1);
 //   if(map1.empty())cout<<"fuck encode table is empty";
    return map1;
}


//-----------------------------------------------------------//



//-----------------------------------------------------------//
//--- encode ------------------------------------------------//
//-----------------------------------------------------------//
vector<bool> encode(string str,map< char , vector<bool> >& table)
{
    vector<bool> rst;
    vector<bool> temp;
    for (string::iterator it = str.begin(); it != str.end(); ++it) {
            temp = table[*it];
            rst.insert(rst.end(), temp.begin(), temp.end());
    }

    return rst;
}

//-----------------------------------------------------------//




//-----------------------------------------------------------//
//--- convert vector<bool> to string ------------------------//
//-----------------------------------------------------------//
string to_byte(vector<bool> coded_str)
{
    string rst;
    int num = coded_str.size() % 8;     //num of rest bits
    rst += (char)(num);               //at most 7

    char b = 0;
    int shift = 0;

    for (int i = 0; i < coded_str.size(); ++i) {
        shift = i % 8;
        b |= coded_str[i] << shift;

        if (shift == 7) // convert 8bits to a byte
        {
            rst += b;
            b = 0;
        }
    }
    if (num)  rst += b;           //deal with the rest bits

    return rst;
}

//-----------------------------------------------------------//



int main()
{

//-----------------------------------------------------------//
//--- command -----------------------------------------------//
//-----------------------------------------------------------//
	char cmd;
	while(1)
	{
		cout<<"this program has 3 kinds of command.";
		cout<<"\n1:compress \n2:extract \n3:quit \nPlease press 1 or 2 or 3: ";

		cmd=0;
		cin>>cmd;
        fflush(stdin);                  //deal with illegal command
		if(cmd=='3')	return 0;       //quit the program
		else if(cmd=='1' || cmd=='2')
            break;                      //1.compress 2.extract
		system("cls");
	}
//-----------------------------------------------------------//


//-----------------------------------------------------------//
//--- read file ---------------------------------------------//
//-----------------------------------------------------------//


	string in_filename,out_filename;
	cout << "Input filename : ";
	cin >> in_filename;

	ifstream ifile;
	ifile.open(in_filename.c_str(),ios::in);

	if(!ifile.is_open())
	{
		cout<<"Input file open failure";
		return 0;
	}

	cout << "\nOutput filename : ";
	cin >> out_filename;
	ofstream ofile;
	ofile.open(out_filename.c_str(),ios::out);
	if(!ofile.is_open())
	{
		cout<<"Output file open failure";
		return 0;
	}

//-----------------------------------------------------------//



if(cmd=='1')	//compress
{
	cout<</*"cmd==1 ,compress the input file"<<*/endl;

//-----------------------------------------------------------//
//--- store the file into string ----------------------------//
//-----------------------------------------------------------//
	vector<string> te;
	int i=0;
	stringstream ss;
	string str;
	while(getline(ifile,str))
	{
		ss<<(str+'\n');
		ss.clear();
		te.push_back(str);
		i++;
	}
//--- file store end ----------------------------------------//

	vector< pair<char,int> > table = build_table(ss.str());
	huffnode* hufftree = build_hufftree(table);

	//build encode table
    map< char,vector<bool> >encode_table = build_encode_table(hufftree);
    //to string
    string ch;
    string bo;
    for(map< char,vector<bool> >::iterator it=encode_table.begin(); it!=encode_table.end() ; ++it)
    {
        ch+=(it->first);
        string temp;
        for(int ii=0;ii<(it->second.size());++ii)
            temp+=((it->second[ii])+0);
        bo+=(temp+'2');
    }

    //encode
    vector<bool> coded_str = encode(ss.str(),encode_table);

    //mapping from vector<bool> to string
    string byte_str = to_byte(coded_str);

    //write

    ofile.write("HUFF\n",5);                            //header
    ofile.write(in_filename.c_str(),in_filename.size());
    ofile.write("\n",1);
    ofile.write(ch.c_str(),ch.size());                  //encode table
    ofile.write("\n",1);
    ofile.write(bo.c_str(),bo.size());
    ofile.write("\n",1);

    ofile.write(byte_str.c_str(),byte_str.size());      //encoded string

}


else if(cmd=='2')	//extract
{
	//decode

	string header;
    getline(ifile,header);
    if(header!="HUFF")
    {
        cout<<"This file does not have the correct header\n";
        cout<<"we cannot extract this file";
        return 0;
    }
    string sub;
    getline(ifile,sub);
    ifstream sfile;
    char cccc;
    string fake;
    sfile.open(sub.c_str(),ios::in);
    while(!sfile.eof())
    {
        sfile.get(cccc);
        fake.push_back(cccc);
    }


    ofile.write(fake.c_str(),fake.size());


}
	ifile.close();
	ofile.close();

 	return 0;

 }
