#include <bits/stdc++.h>
using namespace std;
enum class Color { RED, BLACK };

ofstream outputFile;

class Reservation {
public:
    int patronID;
    int priorityNumber;
    long long timeOfReservation;

    Reservation(int id, int priority, long long timestamp)
        : patronID(id), priorityNumber(priority), timeOfReservation(timestamp) {}
};

class Book {
public:
    int bookID;
    string bookName;
    string authorName;
    bool availabilityStatus;
    int borrowedBy;
    vector<Reservation> reservationHeap;

    Book(int id, const string& name, const string& author, bool availability)
        : bookID(id), bookName(name), authorName(author), availabilityStatus(availability), borrowedBy(-1) {}

    void PrintBookDetails() const {
		outputFile << "BookID = " << bookID << "\n";
		outputFile << "Title = " << bookName << "\"\n";
		outputFile << "Author = " << authorName << "\"\n";
		outputFile << "Availability = \"" << (availabilityStatus ? "Yes" : "No") << "\"\n";
		outputFile << "BorrowedBy = " << (availabilityStatus ? "None" : to_string(borrowedBy)) << "\n";

		if (!reservationHeap.empty()) {
			// Sort reservations by priority and timestamp
			vector<Reservation> sortedReservations = reservationHeap;
			sort(sortedReservations.begin(), sortedReservations.end(),
				 [](const Reservation &a, const Reservation &b) {
					 return make_tuple(a.priorityNumber, a.timeOfReservation) <
							make_tuple(b.priorityNumber, b.timeOfReservation);
				 });

			// Reverse the order of sorted reservations
			//reverse(sortedReservations.begin(), sortedReservations.end());

			outputFile << "Reservations = [";
			for (size_t i = 0; i < sortedReservations.size(); ++i) {
				outputFile << sortedReservations[i].patronID;
				if (i < sortedReservations.size() - 1) {
					outputFile << ", ";
				}
			}
			outputFile << "]\n";
		} else {
			outputFile << "Reservations = []\n";
		}

		outputFile << "\n";
	}


};

class RBTreeNode {
public:
    Book book;
    Color nodeColor;
    RBTreeNode* parent;
    RBTreeNode* left;
    RBTreeNode* right;

    RBTreeNode(const Book& newBook)
        : book(newBook), nodeColor(Color::RED), parent(nullptr), left(nullptr), right(nullptr) {}
};

class RedBlackTree {
private:
    RBTreeNode* root;
    int colorFlipCount = 0; // Counter for color flips

public:
    RedBlackTree() : root(nullptr), colorFlipCount(0) {}

    void InsertBook(const Book& newBook) {
        RBTreeNode* newNode = new RBTreeNode(newBook);
        InsertNode(newNode);
    }

    void PrintBooksInRange(int bookID1, int bookID2) const {
        PrintBooksInRangeHelper(root, bookID1, bookID2);
    }
    
    void PrintBook(int bookID) const {
        PrintBookHelper(root, bookID);
    }
    
    void BorrowBook(int patronID, int bookID, int patronPriority) {
        BorrowBookHelper(root, patronID, bookID, patronPriority);
    }
    
    void ReturnBook(int patronID, int bookID) {
        ReturnBookHelper(root, patronID, bookID);
    }
    
    void DeleteBook(int bookID) {
        DeleteBookHelper(root, bookID);
    }
    
	void FindClosestBook(int targetID) const {
		FindClosestBookHelper(root, targetID);
	}

    int ColorFlipCount() const {
        return colorFlipCount;
    }

private:
    void InsertNode(RBTreeNode* newNode) {
        // Insertion logic based on Red-Black Tree rules
        if (root == nullptr) {
            root = newNode;
            root->nodeColor = Color::BLACK;
        } else {
            RBTreeNode* parent = nullptr;
            RBTreeNode* current = root;

            while (current != nullptr) {
                parent = current;
                if (newNode->book.bookID < current->book.bookID) {
                    current = current->left;
                } else {
                    current = current->right;
                }
            }

            newNode->parent = parent;
            if (newNode->book.bookID < parent->book.bookID) {
                parent->left = newNode;
            } else {
                parent->right = newNode;
            }

            // Additional logic for maintaining Red-Black Tree properties
            FixInsertViolation(newNode);
        }
    }

    void FixInsertViolation(RBTreeNode* node) {
        // Red-Black Tree fix-up logic after insertion
        while (node->parent != nullptr && node->parent->nodeColor == Color::RED) {
            if (node->parent == node->parent->parent->left) {
                RBTreeNode* uncle = node->parent->parent->right;
                if (uncle != nullptr && uncle->nodeColor == Color::RED) {
                    // Case 1: Uncle is red
                    node->parent->nodeColor = Color::BLACK;
                    uncle->nodeColor = Color::BLACK;
                    node->parent->parent->nodeColor = Color::RED;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->right) {
                        // Case 2: Uncle is black, and node is a right child
                        node = node->parent;
                        RotateLeft(node);
                    }
                    // Case 3: Uncle is black, and node is a left child
                    node->parent->nodeColor = Color::BLACK;
                    node->parent->parent->nodeColor = Color::RED;
                    RotateRight(node->parent->parent);
                }
            } else {
                RBTreeNode* uncle = node->parent->parent->left;
                if (uncle != nullptr && uncle->nodeColor == Color::RED) {
                    // Case 1: Uncle is red
                    node->parent->nodeColor = Color::BLACK;
                    uncle->nodeColor = Color::BLACK;
                    node->parent->parent->nodeColor = Color::RED;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->left) {
                        // Case 2: Uncle is black, and node is a left child
                        node = node->parent;
                        RotateRight(node);
                    }
                    // Case 3: Uncle is black, and node is a right child
                    node->parent->nodeColor = Color::BLACK;
                    node->parent->parent->nodeColor = Color::RED;
                    RotateLeft(node->parent->parent);
                }
            }
        }

        root->nodeColor = Color::BLACK; // Ensure the root is always black
        
        // Increment the color flip count when a color change occurs
        if (node->nodeColor == Color::RED) {
            colorFlipCount++;
        }
    }

    void RotateLeft(RBTreeNode* node) {
        // Left rotation in Red-Black Tree
        RBTreeNode* rightChild = node->right;
        node->right = rightChild->left;
        if (rightChild->left != nullptr) {
            rightChild->left->parent = node;
        }
        rightChild->parent = node->parent;
        if (node->parent == nullptr) {
            root = rightChild;
        } else if (node == node->parent->left) {
            node->parent->left = rightChild;
        } else {
            node->parent->right = rightChild;
        }
        rightChild->left = node;
        node->parent = rightChild;
        // Increment the color flip count when a color change occurs
        colorFlipCount++;
    }

    void RotateRight(RBTreeNode* node) {
        // Right rotation in Red-Black Tree
        RBTreeNode* leftChild = node->left;
        node->left = leftChild->right;
        if (leftChild->right != nullptr) {
            leftChild->right->parent = node;
        }
        leftChild->parent = node->parent;
        if (node->parent == nullptr) {
            root = leftChild;
        } else if (node == node->parent->left) {
            node->parent->left = leftChild;
        } else {
            node->parent->right = leftChild;
        }
        leftChild->right = node;
        node->parent = leftChild;
        // Increment the color flip count when a color change occurs
        colorFlipCount++;
    }

    void BorrowBookHelper(RBTreeNode* node, int patronID, int bookID, int patronPriority) {
	  // Find the book with the given bookID
	  while (node != nullptr) {
		if (bookID < node->book.bookID) {
		  node = node->left;
		} else if (bookID > node->book.bookID) {
		  node = node->right;
		} else {
		  // Book found, update availability and borrowedBy
		  if (node->book.availabilityStatus) {
			node->book.availabilityStatus = false;
			node->book.borrowedBy = patronID;
			outputFile << "Book " << bookID << " Borrowed by Patron " << patronID << "\n";
			outputFile << "\n";
		  } else {
			// Book is currently unavailable, create a reservation
			Reservation newReservation(patronID, patronPriority, chrono::duration_cast<chrono::milliseconds>(
				chrono::system_clock::now().time_since_epoch()).count());

			// Find the correct position to insert the reservation based on priority and timestamp
			auto it = upper_bound(node->book.reservationHeap.begin(), node->book.reservationHeap.end(), newReservation,
								 [](const Reservation& a, const Reservation& b) {
								   return make_tuple(a.priorityNumber, a.timeOfReservation) <
										 make_tuple(b.priorityNumber, b.timeOfReservation);
								 });
			node->book.reservationHeap.insert(it, newReservation);
			outputFile << "Book " << bookID << " Reserved by Patron " << patronID << "\n";
			outputFile << "\n";
		  }
		  return;
		}
	  }
	  outputFile << "Book with ID " << bookID << " not found.\n";
	}

    void ReturnBookHelper(RBTreeNode* node, int patronID, int bookID) {
        // Find the book with the given bookID
        while (node != nullptr) {
            if (bookID < node->book.bookID) {
                node = node->left;
            } else if (bookID > node->book.bookID) {
                node = node->right;
            } else {
                // Book found, check if it is borrowed by the specified patron
                if (!node->book.availabilityStatus && node->book.borrowedBy == patronID) {
                    // Book is being returned
                    outputFile << "Book "<<bookID<<" Returned by Patron " << patronID << "\n";
					outputFile << "\n";
    
                    // Update book status
                    node->book.availabilityStatus = true;
                    node->book.borrowedBy = -1;
    
                    // Check if there are reservations
                    if (!node->book.reservationHeap.empty()) {
                        // Assign the book to the patron with the highest priority
                        int reservedPatronID = node->book.reservationHeap.front().patronID;
                        outputFile << "Book "<<bookID<<" Allocated to Patron " << reservedPatronID << "\n";
						outputFile << "\n";
                        // Remove the reservation
                        node->book.reservationHeap.erase(node->book.reservationHeap.begin());
    
                        // Update book status
                        node->book.availabilityStatus = false;
                        node->book.borrowedBy = reservedPatronID;
                    }
    
                    return;
                } else {
                    outputFile << "PatronID " << patronID << " did not borrow BookID " << bookID << ".\n";
                    return;
                }
            }
        }
    
        // Book not found
        outputFile << "Book with ID " << bookID << " not found.\n";
    }

    void PrintBookHelper(const RBTreeNode* node, int bookID) const {
        while (node != nullptr) {
            if (bookID < node->book.bookID) {
                node = node->left;
            } else if (bookID > node->book.bookID) {
                node = node->right;
            } else {
                // Book found, print details
                node->book.PrintBookDetails();
                return;
            }
        }

        // Book not found
        outputFile << "Book " << bookID << " not found in the library.\n";
		outputFile<<"\n";
    }
    
    void PrintBooksInRangeHelper(const RBTreeNode* node, int bookID1, int bookID2) const {
        if (node == nullptr) {
            return;
        }

        PrintBooksInRangeHelper(node->left, bookID1, bookID2);

        if (node->book.bookID >= bookID1 && node->book.bookID <= bookID2) {
            node->book.PrintBookDetails();
        }

        PrintBooksInRangeHelper(node->right, bookID1, bookID2);
    }
    
	void FindClosestBookHelper(const RBTreeNode* node, int targetID) const {
	  int closestDiff = numeric_limits<int>::max();
	  vector<int> closestIDs;
	  vector<const RBTreeNode*> closestNodes;

	  while (node != nullptr) {
		int currentDiff = abs(targetID - node->book.bookID);

		if (currentDiff < closestDiff) {
		  closestDiff = currentDiff;
		  closestIDs.clear();
		  closestNodes.clear();
		  closestIDs.push_back(node->book.bookID);
		  closestNodes.push_back(node);
		} else if (currentDiff == closestDiff) {
		  closestIDs.push_back(node->book.bookID);
		  closestNodes.push_back(node);
		}

		if (targetID < node->book.bookID) {
		  node = node->left;
		} else if (targetID > node->book.bookID) {
		  node = node->right;
		} else {
		  // Exact match found, add it to the list
		  closestIDs.push_back(node->book.bookID);
		  closestNodes.push_back(node);
		  break;
		}
	  }

	  if (!closestIDs.empty()) {
		// Sort the list of closest book IDs
		sort(closestIDs.begin(), closestIDs.end());

		for (size_t i = 0; i < closestIDs.size(); ++i) {
		  // Find the corresponding node in the closestNodes list
		  for (size_t j = 0; j < closestNodes.size(); ++j) {
			if (closestIDs[i] == closestNodes[j]->book.bookID) {
			  closestNodes[j]->book.PrintBookDetails();
			  break;
			}
		  }
		}
	  } else {
		outputFile << "No books found.\n";
	  }
	}
	
    void DeleteBookHelper(RBTreeNode* node, int bookID) {
		// Find the book with the given bookID
		while (node != nullptr) {
			if (bookID < node->book.bookID) {
				node = node->left;
			} else if (bookID > node->book.bookID) {
				node = node->right;
			} else {
				// Book found, delete it
				outputFile << "Book " << node->book.bookID << " is no longer available. "<< "\n";
				outputFile<<"\n";
				// Notify patrons in the reservation list
				if (!node->book.reservationHeap.empty()) {
					outputFile << "Reservations made by Patrons ";
					for (size_t i = 0; i < node->book.reservationHeap.size(); ++i) {
						outputFile << node->book.reservationHeap[i].patronID;
						if (i < node->book.reservationHeap.size() - 1) {
							outputFile << ", ";
						}
					}
					outputFile << " have been cancelled!\n";
					outputFile << "\n";
				}

				// Delete the node
				DeleteNode(node);
				return;
			}
		}

		// Book not found
		outputFile << "Book with ID " << bookID << " not found.\n";
	}


    void DeleteNode(RBTreeNode* node) {
        // Deletion logic for Red-Black Tree
        if (node->left != nullptr && node->right != nullptr) {
            // Node has two children
            RBTreeNode* successor = GetSuccessor(node->right);
            node->book = successor->book;
            DeleteNode(successor);
        } else {
            // Node has at most one child
            RBTreeNode* child = (node->left != nullptr) ? node->left : node->right;
            if (node->nodeColor == Color::BLACK) {
                // If the node is black, fixing the deletion violation
                FixDeleteViolation(node);
            }
            ReplaceNode(node, child);
            delete node;
        }
    }
    
    RBTreeNode* GetSuccessor(RBTreeNode* node) {
        // Find the leftmost node in the subtree
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }
    
    void FixDeleteViolation(RBTreeNode* node) {
        // Red-Black Tree fix-up logic after deletion
        while (node != root && (node == nullptr || node->nodeColor == Color::BLACK)) {
            if (node == node->parent->left) {
                RBTreeNode* sibling = node->parent->right;
                if (sibling->nodeColor == Color::RED) {
                    // Case 1: Sibling is red
                    sibling->nodeColor = Color::BLACK;
                    node->parent->nodeColor = Color::RED;
                    RotateLeft(node->parent);
                    sibling = node->parent->right;
                }
                if ((sibling->left == nullptr || sibling->left->nodeColor == Color::BLACK) &&
                    (sibling->right == nullptr || sibling->right->nodeColor == Color::BLACK)) {
                    // Case 2: Sibling and its children are black
                    sibling->nodeColor = Color::RED;
                    node = node->parent;
                } else {
                    if (sibling->right == nullptr || sibling->right->nodeColor == Color::BLACK) {
                        // Case 3: Sibling is black, sibling's left child is red, and sibling's right child is black
                        sibling->left->nodeColor = Color::BLACK;
                        sibling->nodeColor = Color::RED;
                        RotateRight(sibling);
                        sibling = node->parent->right;
                    }
                    // Case 4: Sibling is black, sibling's right child is red
                    sibling->nodeColor = node->parent->nodeColor;
                    node->parent->nodeColor = Color::BLACK;
                    sibling->right->nodeColor = Color::BLACK;
                    RotateLeft(node->parent);
                    node = root; // Exit the loop
                }
            } else {
                // Symmetric cases for a right child
                RBTreeNode* sibling = node->parent->left;
                if (sibling->nodeColor == Color::RED) {
                    // Case 1
                    sibling->nodeColor = Color::BLACK;
                    node->parent->nodeColor = Color::RED;
                    RotateRight(node->parent);
                    sibling = node->parent->left;
                }
                if ((sibling->right == nullptr || sibling->right->nodeColor == Color::BLACK) &&
                    (sibling->left == nullptr || sibling->left->nodeColor == Color::BLACK)) {
                    // Case 2
                    sibling->nodeColor = Color::RED;
                    node = node->parent;
                } else {
                    if (sibling->left == nullptr || sibling->left->nodeColor == Color::BLACK) {
                        // Case 3
                        sibling->right->nodeColor = Color::BLACK;
                        sibling->nodeColor = Color::RED;
                        RotateLeft(sibling);
                        sibling = node->parent->left;
                    }
                    // Case 4
                    sibling->nodeColor = node->parent->nodeColor;
                    node->parent->nodeColor = Color::BLACK;
                    sibling->left->nodeColor = Color::BLACK;
                    RotateRight(node->parent);
                    node = root; // Exit the loop
                }
            }
        }
        // Increment the color flip count when a color change occurs
        if (node != nullptr && node->nodeColor == Color::BLACK) {
            colorFlipCount++;
        }
    }
    
    void ReplaceNode(RBTreeNode* node, RBTreeNode* child) {
        // Replace the node with its child
        if (node->parent == nullptr) {
            // Node is the root
            root = child;
        } else if (node == node->parent->left) {
            node->parent->left = child;
        } else {
            node->parent->right = child;
        }
        if (child != nullptr) {
            child->parent = node->parent;
        }
    }
    
    void RotateLeftD(RBTreeNode* node) {
        // Left rotation
        RBTreeNode* rightChild = node->right;
        node->right = rightChild->left;
        if (rightChild->left != nullptr) {
            rightChild->left->parent = node;
        }
        rightChild->parent = node->parent;
        if (node->parent == nullptr) {
            root = rightChild;
        } else if (node == node->parent->left) {
            node->parent->left = rightChild;
        } else {
            node->parent->right = rightChild;
        }
        rightChild->left = node;
        node->parent = rightChild;
        // Increment the color flip count when a color change occurs
        colorFlipCount++;
    }
    
    void RotateRightD(RBTreeNode* node) {
        // Right rotation
        RBTreeNode* leftChild = node->left;
        node->left = leftChild->right;
        if (leftChild->right != nullptr) {
            leftChild->right->parent = node;
        }
        leftChild->parent = node->parent;
        if (node->parent == nullptr) {
            root = leftChild;
        } else if (node == node->parent->left) {
            node->parent->left = leftChild;
        } else {
            node->parent->right = leftChild;
        }
        leftChild->right = node;
        node->parent = leftChild;
        // Increment the color flip count when a color change occurs
        colorFlipCount++;
    }
    
};

int main(int argc, char* argv[]) {
    RedBlackTree bookTree;
	
	if (argc != 2) {
		cerr << "Usage: program_name input_file_name\n";
		return 1;
	}
	
	// Store the input file name from the command-line argument
	string inputFileName = argv[1];
	
    // Read input from a file
    ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        cerr << "Error opening the input file.\n";
        return 1;
    }
	
	string OutputFileName = inputFileName.substr(0, inputFileName.find(".")) + "_output_file.txt";
	// setting up the output file
	outputFile.open(OutputFileName);
	
    string line;
    while (getline(inputFile, line)) {
        // Remove anything after the closing parenthesis
        line = line.substr(0, line.find(")"));
        
        // Split the line into command and arguments
        size_t openingBracket = line.find("(");
        string command = line.substr(0, openingBracket);
        string argString = line.substr(openingBracket + 1);

        // Split the arguments into individual strings
        vector<string> args;
        stringstream ss(argString);
        string argument;
        while (getline(ss, argument, ',')) {
            args.push_back(argument);
        }

        // Process the command
        if (command == "InsertBook") {
            int bookID = stoi(args[0]);
            string bookName = args[1].substr(1, args[1].size() - 2);
            string authorName = args[2].substr(1, args[2].size() - 2);
            bool availability = (args[3].find("Yes") != string::npos);
			//outputFile<<availability<<endl;
            Book newBook(bookID, bookName, authorName, availability);
            bookTree.InsertBook(newBook);
        } else if (command == "BorrowBook") {
            int patronID = stoi(args[0]);
            int bookID = stoi(args[1]);
            int patronPriority = stoi(args[2]);
			//outputFile<<patronID<<"::"<<bookID<<"::"<<patronPriority<<endl;
            bookTree.BorrowBook(patronID, bookID, patronPriority);
        } else if (command == "DeleteBook") {
            int bookID = stoi(args[0]);
            bookTree.DeleteBook(bookID);
        } else if (command == "PrintBooks") {
            int bookID1 = stoi(args[0]);
            int bookID2 = stoi(args[1]);
            bookTree.PrintBooksInRange(bookID1, bookID2);
        } else if (command == "PrintBook") {
            int bookID = stoi(args[0]);
            bookTree.PrintBook(bookID);
        } else if (command == "FindClosestBook") {
			int targetId = stoi(args[0]);
			bookTree.FindClosestBook(targetId);
		} else if (command == "ReturnBook") {
            int patronID = stoi(args[0]);
            int bookID = stoi(args[1]);
            bookTree.ReturnBook(patronID, bookID);
        } else if (command == "ColorFlipCount") {
            outputFile << "Color Flip Count: " << bookTree.ColorFlipCount() << "\n";
			outputFile << "\n";
        } else if (command == "Quit") {
            outputFile << "Program Terminated!!\n";
            break;
        } else {
            cerr << "Unknown command: " << command << "\n";
        }
    }

    inputFile.close();
	outputFile.close();

    return 0;
}




