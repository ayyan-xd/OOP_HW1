#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

class Player {
private :
    int id;
    string name;
    int level;
    string type;
    int arrivalOrder;
    public:
        Player()
        {
            id = -1;
            name = " ";
            level = 0;
            type = "Normal";
            arrivalOrder = -1;
        }
        Player(int p_id, string p_name, int p_level, string type, int p_order)
        {
            id = p_id;
            name = p_name;
            if(type == "VIP")level = p_level;
            else level = 0;
            this->type = type;
            arrivalOrder = p_order;
            
        }
        Player(int p_id, string p_name, int p_level, string type)
        {
            id = p_id;
            name = p_name;
            if(type=="VIP")level = p_level;
            else level = 0;
            this->type = type;
        }
        int getID() { return id; }
        string getName() { return name; }
        int getLevel() { return level; }
        string getType() { return type; }
        int getArrival(){ return arrivalOrder; }
        void setLevel(int newLevel) { 
            level = newLevel; 
        }
        void setArrival(int newArrival) {
            arrivalOrder = newArrival;
        }
        
};


enum ActionType {
    REGISTER,
    PLAY,
    BOOST,
    WITHDRAW,
    UNDO,
    STATUS
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
class HistoryNode {
    public :
    ActionType action;
    Player* player;
    int oldLevel;
    HistoryNode* next;
    HistoryNode(ActionType a, Player* p, int level = 0) {
        action = a;
        player = p;
        oldLevel = level;
        next = nullptr;
    }
};

string lastAction;
class UndoStack {
    private:
    HistoryNode* top;
    public:
    UndoStack() {
        top = nullptr;
    }
    void push(ActionType action, Player* player, int oldLevel = 0) {
        HistoryNode* newNode = new HistoryNode(action, player, oldLevel);
        newNode->next = top;
        top = newNode;
    }
    void pop() {
        if (top == nullptr) return;
        HistoryNode* temp = top;
        top = top->next;
        delete temp;
    }
    HistoryNode* peek() {
        return top;
    }
    bool isEmpty() {
        return (top == nullptr);
    }
};

UndoStack undoStack;

class NormalQueue {
    private:
    Node* front;
    Node* rear;
    int size;

public:
    NormalQueue() {
        front = rear = nullptr;
        size = 0;
    }
    void push(Player* p,bool isUndo = false) {
        Node* newNode = new Node(p);
        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        p->setArrival(size);
        size++;
        if(!isUndo)
        undoStack.push(REGISTER, p);
        lastAction = "NormalQueue";
    }
    void pop(bool isUndo = false) {
        if (front == nullptr) return;
        Node* temp = front;
        front = front->next;
        if(!isUndo)
        undoStack.push(PLAY, temp->player);
        delete temp;
        size--;
        if (front == nullptr) rear = nullptr;
        lastAction = "NormalQueue";
    }
    Player* peek() {
        if (front == nullptr) return nullptr;
        return front->player;
    }
    bool isEmpty() {
        return (front == nullptr);
    }
    void Play(bool isUndo = false) {
        if (front == nullptr) return;
        Player* currentPlayer = front->player;
        cout << "Playing: " << currentPlayer->getName() << " (ID: " << currentPlayer->getID() << ", Level: " << currentPlayer->getLevel() << ", Special: " << currentPlayer->getType() << ")" << endl;
        pop(isUndo);
        lastAction = "NormalQueue";
    }
    void pushfront(Player* p) {
        Node* newNode = new Node(p);
        if (rear == nullptr) {
            rear = front = newNode;
        } else {
            newNode->next = front;
            front = newNode;
        }
        p->setArrival(size);
        size++;
    }
    void display() {
        if (front == nullptr) {
            cout << "Empty" << endl;
            return;
        }
        Node* current = front;
        while (current != nullptr) {
            cout << current->player->getName() << " (ID: " << current->player->getID() 
                 << ", Level: " << current->player->getLevel() << ", Type: " 
                 << current->player->getType() << ")";
            if (current->next != nullptr) cout << " -> ";
            current = current->next;
        }
        cout << endl;
    }
};


class VipQueue {
    private :
        Node *front;
        Node *rear;
        int size;
    public :
    VipQueue() {
        front = rear = nullptr;
        size = 0;
    }
    bool isEmpty() {
        return (front == nullptr);
    }
    int findPlaceinQueue(Player* p){
        if(front ==nullptr)
            return 0;
        Node* current = front;
        int pos = 0;
        while(current!= nullptr){
            if(p->getLevel() < current->player->getLevel()){
                pos++;
            }
            else if(p->getLevel() == current->player->getLevel()){
                if(p->getArrival() > current->player->getArrival()){
                    pos++;
                }
                else break;
            }
            else break;
            current=current->next;
        }
        return pos;
    }
    void Register(Player* p,bool isUndo=false){
        int pos = findPlaceinQueue(p);
        Node* newNode = new Node(p);
        
        if(front == nullptr) {
            front = rear = newNode;
        }
        else if(pos == 0) {
            newNode->next = front;
            front = newNode;
        }
        else {
            Node *current = front;
            for (int i = 0; i < pos-1; i++){
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
            if(newNode->next == nullptr) rear = newNode;
        }
        
        if(!isUndo) undoStack.push(REGISTER, p);
        p->setArrival(size);
        size++;
        lastAction = "VipQueue";
    }
    void Play(bool isUndo=false){
        if(front == nullptr) return;
        Player* currentPlayer = front->player;
        cout << "Playing: " << currentPlayer->getName() << " (ID: " << currentPlayer->getID() << ", Level: " << currentPlayer->getLevel() << ", Special: " << currentPlayer->getType() << ")" << endl;
        Node* temp = front;
        front = front->next;
        if(!isUndo) undoStack.push(PLAY, currentPlayer);
        temp->player = nullptr;
        delete temp;
        size--;
        if (front == nullptr) rear = nullptr;
        lastAction = "VipQueue";
    }
    void pushfront(Player* p) {
        Node* newNode = new Node(p);
        if (rear == nullptr) {
            rear = front = newNode;
        } else {
            newNode->next = front;
            front = newNode;
        }
    }
    void Boost(int id, int newLevel,bool isUndo=false){
        Node* current = front;
        while(current!= nullptr){
            if(current->player->getID()==id){
                int oldLevel = current->player->getLevel();
                current->player->setLevel(newLevel);
                if(!isUndo) undoStack.push(BOOST, current->player, oldLevel);
                lastAction = "VipQueue";
                return;
            }
            current=current->next;
        }
    }
    void Withdraw(int id,bool isUndo=false){
        if(front == nullptr) return;
        
        Node* current = front;
        if(front->player->getID()==id){
            Node* temp = front;
            front = front->next;
            if(!isUndo) undoStack.push(WITHDRAW, temp->player);
            delete temp;
            size--;
            if (front == nullptr) rear = nullptr;
            lastAction = "VipQueue";
            return;
        }
        
        while(current->next != nullptr && current->next->player->getID() != id){
            current = current->next;
        }
        
        if(current->next == nullptr) return;
        
        Node* toDelete = current->next;
        current->next = current->next->next;
        if(toDelete == rear){
            rear = current;
        }
        if(!isUndo) undoStack.push(WITHDRAW, toDelete->player);
        delete toDelete;
        size--;
        lastAction = "VipQueue";
    }
    void display() {
        if (front == nullptr) {
            cout << "Empty" << endl;
            return;
        }
        Node* current = front;
        while (current != nullptr) {
            cout << current->player->getName() << " (ID: " << current->player->getID() 
                 << ", Level: " << current->player->getLevel() << ", Type: " 
                 << current->player->getType() << ")";
            if (current->next != nullptr) cout << " -> ";
            current = current->next;
        }
        cout << endl;
    }

};

void Undo(VipQueue &pq, NormalQueue &nq){
    if(undoStack.isEmpty()) {
        cout << "No actions to undo." << endl;
        return;
    }
    HistoryNode* lastActionNode = undoStack.peek();
    ActionType action = lastActionNode->action;
    Player* player = lastActionNode->player;
    undoStack.pop();
    if(lastAction == "VipQueue"){
        if(action == REGISTER){
            pq.Withdraw(player->getID(), true);
        }
        else if(action == PLAY){
            pq.pushfront(player);
        }
        else if(action == BOOST){
            pq.Boost(player->getID(), lastActionNode->oldLevel, true);
        }
        else if(action == WITHDRAW){
            pq.Register(player, true);
        }
    }
    else if(lastAction == "NormalQueue"){
        if(action == REGISTER){
            nq.pop(true);
        }
        else if(action == PLAY){
            nq.pushfront(player);
        }
    }

}

void displayStatus(VipQueue& vipQ, NormalQueue& normalQ) {
    cout << "VIP Queue: ";
    vipQ.display();
    cout << "Normal Queue: ";
    normalQ.display();
}

int main() {
    VipQueue vipQueue;
    NormalQueue normalQueue;
    unordered_map<int, Player*> players;
    int Q;
    cout << "Enter number of operations: ";
    cin >> Q;
    
    for(int i = 0; i < Q; i++) {
        string command;
        cout << "\nEnter command (REGISTER/PLAY/BOOST/WITHDRAW/UNDO/STATUS): ";
        cin >> command;
        
        if(command == "REGISTER") {
            int id, level;
            string name, type;
            cout << "Enter id name level type: ";
            cin >> id >> name >> level >> type;
            
            Player* newPlayer = new Player(id, name, level, type, i);
            players[id] = newPlayer;
            
            if(type == "VIP") {
                vipQueue.Register(newPlayer);
            } else {
                normalQueue.push(newPlayer);
            }
            cout << "Player registered successfully." << endl;
        }
        else if(command == "PLAY") {
            if(!vipQueue.isEmpty()) {
                vipQueue.Play();
            } else if(!normalQueue.isEmpty()) {
                normalQueue.Play();
            } else {
                cout << "No players in queue." << endl;
            }
        }
        else if(command == "BOOST") {
            int id, x;
            cout << "Enter id and boost amount: ";
            cin >> id >> x;
            if(players.find(id) != players.end()) {
                Player* p = players[id];
                if(p->getType() == "VIP") {
                    int newLevel = p->getLevel() + x;
                    vipQueue.Boost(id, newLevel);
                    cout << "Player " << id << " boosted to level " << newLevel << endl;
                } else {
                    cout << "Cannot boost Normal players." << endl;
                }
            } else {
                cout << "Player not found." << endl;
            }
        }
        else if(command == "WITHDRAW") {
            int id;
            cout << "Enter id to withdraw: ";
            cin >> id;
            if(players.find(id) != players.end()) {
                Player* p = players[id];
                if(p->getType() == "VIP") {
                    vipQueue.Withdraw(id);
                    cout << "Player " << id << " withdrawn from queue." << endl;
                } else {
                    cout << "Cannot withdraw Normal players (use PLAY instead)." << endl;
                }
            } else {
                cout << "Player not found." << endl;
            }
        }
        else if(command == "UNDO") {
            Undo(vipQueue, normalQueue);
        }
        else if(command == "STATUS") {
            displayStatus(vipQueue, normalQueue);
        }
        else {
            cout << "Invalid command." << endl;
        }
    }
    
    return 0;
}
