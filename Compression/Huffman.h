#include "MinHeap.h"

//Hàm tạo 1 node mới với dữ liệu là gồm ký tự data và tần suất freq
Node* CreateNode(char data, int freq);
//Hàm kiểm tra 1 node có phải node lá không
bool IsLeaf(Node* node);

//Hàm xây dựng cây Huffman dựa trên dữ liệu ban đầu là các ký tự và tần suất
Node* BuildHuffmanTree(vector<Node*>& datas);
//Hàm giải phóng các node của cây
void DeleteTree(Node* root);
//Hàm xây dựng codebook tức mảng các ký tự và dãy bit của chúng
void BuildCodeBook(Node* root, vector<string>& codeBook, string cur = "");
//Hàm duyệt qua cây huffman theo chuỗi bit “s” để tìm ký tự và gán vào biến c, //nếu tìm thấy trả về 1 và không tìm thấy trả về 0
bool Traverse(Node*& node, char& bit, char& c);
