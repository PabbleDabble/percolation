/*
Patrick Vielhaber
PTV1
1640823
10_22_2014 
Percolation
*/

using namespace std;

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <iomanip>

/*
#include <stdlib.h>
#include <sstream>
#include <string>
#include <string.h>
*/

void print_board(int *arr, int size);
int *make_board(float prob, int size);
int *cluster_board(int *arr, int size);
int *fix_roots(int *arr, int size);
int find(int *arr, int to_find);


int main(int argc, char *argv[]) {
	//cout << endl << "------------NEW PROGRAM------------" << endl << endl; 

	float prob;
	int runs;
	int size;
	
	bool bugger;
	bugger = false;
	
	string output;
	output = "short";
//	output = "long";	
	
	
	time_t my_seed = time(NULL);  //This needs to be in for production
	//my_seed = 1413314644;

	//cout << "Seeded with: " << my_seed << endl;
	srand(my_seed);

	
	
	//----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------	
	//This means they did not send any inputs in, and will exit the program
	if (argc != 2 && argc != 4) {
		cout << "Wrong input format!" << endl << endl;
		cout << "Please run the program with one argument (a text file containing a percolation matrix)." << endl
			<< "Or inputs in this format: p n s" << endl
			<< "Where:" << endl
			<< "     p - percolation probability, value between 0 and 1 (double, with up to 5 decimal places)" << endl
			<< "     n - number of runs (integer greater than zero)" << endl
			<< "     s - size (length or width) of the square board (integer greater than zero)" << endl;
		exit(1);
	}
	//----------------------------------------------------------------------------------	
	//----------------------------------------------------------------------------------	
	//This is if they gave you a file to read in
	if (argc==2) {
		string filename(argv[1]);
		if (output == "short")
			cout << "         file:" << filename;
		else
			cout << "----READING IN FILE----" << filename << endl;

		//FIND OUT HOW BIG THE FILE IS...
		int size = 0;
		string line;
		
		//Open the file
		ifstream read_file;
		read_file.open (filename.c_str());

		//-----------------------------------------------------		
		//Count how many characters
		//-----------------------------------------------------		
		while (!read_file.eof()){
			int x;
			read_file >> x;
			size++;
		}
		size--;  //Reset one because of the /0 endline
		
		
		
		//-----------------------------------------------------
		//Test and make sure the file contained a square matrix
		//-----------------------------------------------------		
		double test_sqrt;
		test_sqrt = sqrt(size);
		int int_root = int(test_sqrt);
		//cout << "Size check: " << size << endl;				
		//cout << "Root check: " << int_root << endl;
		//cout << "Root squared check:" << int_root * int_root << endl;
		if (int_root * int_root != size){
			cout << "File did not contain a square matrix. Ending program." << endl;
			read_file.close();
			return 1;	
		} 
		
		//Adjuest the size
		size = int_root;
		//Reset the file field reader		
		read_file.clear(); //Reset the EOF()
		read_file.seekg(0, ios::beg);
		
		//-----------------------------------------------------
		//Make the text file into a new board
		//-----------------------------------------------------	
		int *user_board = new int[size * size];
		int i = 0;
		while (!read_file.eof()){
			int x;
			read_file >> x;
			//cout << x;
			user_board[i] = x;
			i++;
		}		
		//Close the file
		read_file.close();
		
		
		//-----------------------------------------------------		
		//CHECK THE PERCOLATION
		//-----------------------------------------------------		
		int *cluster = new int[size * size];
		cluster = cluster_board(user_board, size);
		
		if (bugger) {
			print_board(user_board,size);
			cout << endl;
			print_board(fix_roots(cluster,size),size);
		}
		
		//Don't actually need to ever run this for now
		//This would be for coloring I guess
		//fix_roots(cluster,size);
		
		//Tell the user 
		if (output == "short")
			cout << " -- there are " << cluster[size*size + 1] + cluster[size*size + 2] << " total clusters" << endl;
		else {
			cout << "There are " << cluster[size*size + 1] << " open clusters." << endl;
			cout << "There are " << cluster[size*size + 2] << " closed clusters." << endl;				
			cout << "There are " << cluster[size*size + 1] + cluster[size*size + 2] << " total clusters." << endl;
			cout << "This board does " << ((cluster[size*size + 3] != 0) ? ("") : ("NOT ")) << "percolate." << endl;
		}
	}
	//----------------------------------------------------------------------------------	
	//----------------------------------------------------------------------------------
	if (argc==4) {
		if (output != "short")		
			cout << endl << "------GENERATING RUNS------" << endl;
			
		prob = atof(argv[1]);
		runs = atoi(argv[2]);
		size = atoi(argv[3]);
		
		int successful_percs = 0;
		double percolation_rate = 0.0;


		if (output == "short")
			cout << "         p=" << prob << ", n=" << runs << ", s=" << size << "; ";
		else {
			cout << "This will create " << runs << " board" << ((runs != 1) ? ("s") : ("")) << "." << endl;
			cout << "The board" << ((runs != 1) ? ("s") : ("")) << " will be " << size << " by " << size << "." << endl;
			cout << ((runs != 1) ? ("Each") : ("The")) << " board will have a percolation probability of " << prob << "." << endl;
			cout << "------BEGINNING CALCULATIONS------" << endl << endl;
		}
		
		for (int i = 0; i < runs; i++){		
			int *tester = new int[size * size];
			tester = make_board(prob, size);
			//print_board(tester,size);			
			int *cluster = new int[size * size];	
			cluster = cluster_board(tester, size);
			
			if (cluster[size*size + 3] != 0) {
				successful_percs++;
			}
			
			//Delete them afterwards to reuse memory
			delete [] tester;
			delete [] cluster;

			//Don't do this during the multiple run setup
			if (1 == 2) {
				cout << "There are " << cluster[size*size + 1] << " open clusters." << endl;
				cout << "There are " << cluster[size*size + 2] << " closed clusters." << endl;				
				cout << "There are " << cluster[size*size + 1] + cluster[size*size + 2] << " total clusters." << endl;				
				cout << "This board does " << ((cluster[size*size + 3] != 0) ? ("") : ("NOT ")) << "percolate." << endl;
				print_board(cluster,size);
				cout << endl;
				print_board(fix_roots(cluster,size),size);
			}
		}

		//cout << "Double division: " << successful_percs << " / " << runs << endl;
		//Do the percolation division for the user
		percolation_rate = double(successful_percs) / double(runs);
		if (output == "short")
			cout << "percolation rate: " << setprecision(5) << fixed << percolation_rate << endl;
		else {
			cout << ((runs !=1) ? ("All runs have") : ("Your run has ")) << "been completed." << endl;
			cout << setprecision(5) << fixed << percolation_rate << endl;
		}
	}
	if (output != "short")
		cout << endl;		
	return 0;
}








//This is the union find function
int *cluster_board(int *board, int size){
	//I make the board with 2 extra sizes to pass back the cluster count
	//And if it percolated or not
	bool c_bug = false;  //This turns on/off words to say what was happening for debugging
	int *clustered = new int [size * size + 3];
	int prev_counter = 0;

	clustered[0] = 0;  //The first item in the board will always be the first root
	int counter_open = 0;  
	int counter_closed = 0;  
	
	//The first corner is always the first root
	if (board[0] == 1)
		counter_open++;
	else 
		counter_closed++;
		
	prev_counter = counter_open + counter_closed;
	for (int i = 1; i < size * size; i++){
	
	
		//This is for debugging, keep this off
		if (prev_counter != (counter_open + counter_closed) && 1==2){
			cout << "Open: " << counter_open << "  Closed: " << counter_closed << "   Element: " << i-1 << endl;		
			prev_counter = (counter_open + counter_closed);
		}
		
		
		//--------------------------------------------------
		//EDGESEDGESEDGESEDGESEDGESEDGESEDGESEDGESEDGESEDGES
		//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
		//--------------------------------------------------
		//Element is on top row - can only look left
		if (i < size ){ 

			if (c_bug) cout << endl << "top-edge---" << ((counter_open + counter_closed > 9) ? "" : " ") << counter_open + counter_closed << " / " << i;
			if (board[i] == board[i-1]){ //Then they are connected from the left
				clustered[i] = find(clustered, i-1);
			} else { //This would be a new root
				clustered[i] = i;
				if (board[i] == 1)
					counter_open++;
				else 
					counter_closed++;
			}
		} 
		//---------------------------------------------------		
		//Element is on the left edge, can only look up
		else if (i % size == 0){
			if (c_bug) cout << endl << "left-edge--" << ((counter_open + counter_closed > 9) ? "" : " ") << counter_open + counter_closed << " / " << i;
	
			if (board[i] == board[i-size]){ //Then they are connected from the left
				clustered[i] = find(clustered, i-size);
			} else { //This would be a new root
				clustered[i] = i;
				if (board[i] == 1)
					counter_open++;
				else 
					counter_closed++;
			}
		}
		//--------------------------------------------------
		//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		//EDGESEDGESEDGESEDGESEDGESEDGESEDGESEDGESEDGESEDGES
		//--------------------------------------------------

		
		//---------------------------------------------------
		//Element has a left and an up
		else {
			//Both connected on the board
			if ((board[i] == board[i-1]) && (board[i] == board[i-size])){
				//If the one to the left and above were both rooted, and the same
				if (find(clustered, i-1) == find(clustered,i-size)) {
					//This was more common than I thought, don't decrement the root!!
					//cout << "xxx---------------------------------------------" << i << endl;
					clustered[i] = find(clustered, i-1);
				} else { //This is when it's connected to left and up, but there are DIFFERENT ROOTS
				
					//Find whichever root is smaller, make that, and the old other root that one
					if (c_bug) cout << endl << "both-chg---" << ((counter_open + counter_closed > 9) ? "" : " ") << counter_open + counter_closed << " / " << i;
					if (board[i] == 1)
						counter_open--;
					else 
						counter_closed--;
					//The root of the element to the left is less
					if (find(clustered, i-1) < find(clustered,i-size)) {
						//Put the root of the left element in the current root
						clustered[i] = find(clustered, i-1);
						//And connect the above element's root to the element to the left's root
						clustered[find(clustered,i-size)] = find(clustered, i-1);
					//The root of the element above is less						
					} else if (find(clustered,i-size) < find(clustered, i-1)){
						//Put the root of the element above in the current element
						clustered[i] = find(clustered, i-size);
						//And connect the left elements root, to the element above's root
						clustered[find(clustered,i-1)] = find(clustered, i-size);
					}
				}
			}
			
			else if (board[i] == board[i-1]) { //Only left connected
				clustered[i] = find(clustered,i-1);
				if (c_bug) cout << endl << "left-------" << ((counter_open + counter_closed > 9) ? "" : " ") << counter_open + counter_closed << " / " << i;
			} else if (board[i] == board[i-size]){ //Only up connected			
				clustered[i] = find(clustered,i-size);
				if (c_bug) cout << endl << "up---------" << ((counter_open + counter_closed > 9) ? "" : " ") << counter_open + counter_closed << " / " << i;			
			} else { //Neigher connected, new root
				clustered[i] = i;
				if (board[i] == 1)
					counter_open++;
				else 
					counter_closed++;
				if (c_bug) cout << endl << "none-------" << ((counter_open + counter_closed > 9) ? "" : " ") << counter_open + counter_closed << " / " << i;
			}
		}		
		
		//Debugging, this will print the clustered matrix every time
		if (i >= 5 && i <= 7 && 1==7) {
			cout << "Run: " << i << "  Clusters: " << counter_open + counter_closed <<  endl;		
			print_board(clustered,size);
		}
	}	
	
	clustered[size * size + 1] = counter_open;
	clustered[size * size + 2] = counter_closed;
	clustered[size * size + 3] = 0;  //Assume it didn't percolate
	
	for (int i = size * (size-1); i < size * size; i++){
		if (find(clustered, i) < size && board[i] == 1) {
			clustered[size * size + 3] = 1;
			//cout << "YES!!!!!!" << endl;
			i = size * size; //Kill the loop
		}
	}
	//Return the grouped array	
	return clustered;
}

//This runs through and matches all the children to the parents
//This could be used for the coloring
//I'm not even sure the do/while loop is needed
//I thought it would need it in case there were multiple disjointed roots
//But I've only seen it run twice ever
int *fix_roots(int *arr, int size){
	int colors[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20}; //Use this later
	bool changed = false;
	int counter = 1;
	
	//Make them negative and switch by one, just to remove the root value, 
	//I dunno, this feels super sloppy
	for (int i = 0; i < size * size; i++)
		arr[i] = (arr[i] + 1) * -1;
		
	//Keep finding lost children and matching them with parents
	do{
		changed = false;
		for (int i = 0; i < size * size; i++){
			if (arr[i] < 0){
				if ((arr[i] * -1) - 1 == i) //This is a root
					arr[i] = counter++;		
				else { //Otherwise just put in that parent
					arr[i] = arr[arr[i] * -1 - 1];
				}
				changed = true;
			}
		}
	} while (changed);
	counter--; //Remove one from the beginning
	return arr;
}

//This will find the root, just more convenient to have out of the cluster function
int find(int *arr, int to_find){
	while (arr[to_find] != to_find)
		to_find = arr[to_find];
	return to_find;
}

//This makes a square board, with percolation probabilty
int *make_board(float prob, int size){
	//cout << "PROB " << prob << endl;	
	int *arr = new int [size * size];

	for (int i = 0; i < size * size; i++){
		//I didn't know many decimal places the probability would have...?
		if (rand() % 100000 < prob * 100000)
			arr[i] = 1;
		else 
			arr[i] = 0;
	}
	return arr;
}

//This will print simple boards, big board, or boards w/out one/zero entries will be weird
void print_board(int *arr, int size){
	cout << "------Board Print------" << endl;
	for (int i = 0; i < size * size; i++){
		if (arr[i] > 99)
			cout << " ";		
		else if (arr[i] > 9)
			cout << "  ";
		else if (arr[i] < 0 && arr[i] > -10)
			cout << "  ";
		else if (arr[i] < -9)
			cout << " ";		
		else 
			cout << "   ";
			
		cout << arr[i];
		if ((i+1) % size == 0) 
			cout << endl;
	}
	cout << endl;
}
