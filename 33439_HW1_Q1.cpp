#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;
enum playerType {VIP, NORMAL};
enum commandType {REGISTER, PLAY, BOOST, WIITHDRAW};
class Player {
    private :
    int id;
    string name;
    int level;
    playerType type;
    int arrivalOrder;
    bool withdrawn;
    public :
    Player() {
        id = -1;
        name = "";
        level = 0;
        type = NORMAL;
        arrivalOrder = -1;
        withdrawn = false;
    }
    Player(int p_id, string p_name, int p_level, playerType p_type, int p_order) {
        id = p_id;
        name = p_name;
        level = p_level;
        type = p_type;
        arrivalOrder = p_order;
        withdrawn = false;
    }
    int getID() {
        return id;
    }
    string getName() {
        return name;
    }
    int getLevel() {
        return level;
    }
    playerType getType() {
        return type;
    }
    int getArrival(){
        return arrivalOrder;
    }
    bool isWithdrawn() {
        return withdrawn;
    }
    void setLevel(int newLevel) {
        level = newLevel;
    }
    void setWithdrawn(bool status) {
        withdrawn = status;
    } 
};
    class Node {
        public :
      Player* player;
      Node* next;
    Node(Player* p) {
        player = p;
        next = nullptr;
    } 
};
    class NormalPlayer {
     private :
     Node* front;
     Node* rear;
     public :        
    NormalPlayer() {
        front = rear = nullptr;
    }
    void push(Player* p) {
        Node* newNode = new Node(p);
        if (rear == nullptr) front = rear = newNode;
        else {
            rear->next = newNode;
            rear = newNode;
        }
    }
    void pushFront(Player* p) {
        Node* newNode = new Node(p);
        if (rear == nullptr) rear = front;
        else {
            newNode->next = front;
            front = newNode;
        }
    }
    Player* pop() {
        if (front == nullptr) return nullptr;
        Node* temp = front;
        Player* plyr = temp->player;
        front = front->next;
        delete temp;
        return plyr;
    }
    Player* peek() {
        if (front == nullptr) return nullptr;
        return front->player;
    }
    bool isEmpty() {
        return (front == nullptr);
    }
};
class VipPlayer {
    private :
        vector <Player*> heap;
        unordered_map <int, int> idToIdx;
    bool isHigherPriority(Player* p1, Player* p2) {
        if (p1->getLevel() != p2->getLevel()) return (p1->getLevel() > p2->getLevel());
        return (p1->getArrival() < p2->getArrival());
    }
    void swapPlayers(int a, int b) {
        int temp = a;
        a = b;
        b = temp;
        idToIdx[heap[a]->getID()] = a;
        idToIdx[heap[b]->getID()] = b;
    } 
    public :
    void push(Player* p) {
        
    }
};