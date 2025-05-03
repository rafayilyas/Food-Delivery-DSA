#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include<climits>
#include <sstream>
#include <unordered_map>
#include<algorithm>
using namespace std;

// FoodItem Class with Category
class FoodItem
{
public:
    int id;
    string name;
    double price;
    string category; // New category attribute

    FoodItem(int id, string name, double price, string category)
        : id(id), name(name), price(price), category(category) {}
};

// TreeNode Class for Menu (BST)
class TreeNode
{
public:
    FoodItem *data;
    TreeNode *left;
    TreeNode *right;
    TreeNode(FoodItem *item) : data(item), left(nullptr), right(nullptr) {}
};

// MenuTree Class (BST) with Category Filter
class MenuTree
{
    TreeNode *root;

    // Insert items based on ID
    TreeNode *insert(TreeNode *node, FoodItem *item)
    {
        if (!node)
            return new TreeNode(item);
        if (item->id < node->data->id) // Compare by ID
            node->left = insert(node->left, item);     //insert in left as small value or id
        else
            node->right = insert(node->right, item);    //insert in right as small value or id
        return node;
    }

    // In-order traversal to display items in ascending order of ID
    void inOrder(TreeNode *node, string category)
    {
        if (!node)
            return;
        inOrder(node->left, category);                  //inorder printing of left data
        if (node->data->category == category)
        {
            cout << node->data->id << ". " << node->data->name << " - $" << node->data->price << endl;
        }
        inOrder(node->right, category);                   //inorder printing of right data
    }

    void saveToFile(TreeNode *node, ofstream &file)
    {
        if (!node)
            return;
        saveToFile(node->left, file);
        file << node->data->id << "," << node->data->name << "," << node->data->price << "," << node->data->category << endl;
        saveToFile(node->right, file);
    }

public:
    MenuTree() : root(nullptr) {}

    void loadFromFile(const string &filename)
    {
        ifstream file(filename);
        if (!file)
        {
            cout << "Menu file not found. A new file will be created when saving.\n";
            return;
        }
        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            int id;
            string name, category;
            double price;

            ss >> id;
            ss.ignore();
            getline(ss, name, ',');
            ss >> price;
            ss.ignore();
            getline(ss, category);

            addFoodItem(new FoodItem(id, name, price, category));
        }
        file.close();
    }

    void saveToFile(const string &filename)
    {
        ofstream file(filename, ios::trunc);
        if (!file)
        {
            cout << "Error saving menu to file.\n";
            return;
        }
        saveToFile(root, file);
        file.close();
        cout << "Menu saved successfully to " << filename << endl;
    }

    void addFoodItem(FoodItem *item)
    {
        root = insert(root, item);
    }

    void removeFoodItem(int id)
    {
        root = remove(root, id);
    }

    TreeNode *remove(TreeNode *node, int id)
    {
        if (!node)
            return node;
        if (id < node->data->id)
            node->left = remove(node->left, id);
        else if (id > node->data->id)
            node->right = remove(node->right, id);
        else
        {
            if (!node->left)
            {
                TreeNode *temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right)
            {
                TreeNode *temp = node->left;
                delete node;
                return temp;
            }
            TreeNode *temp = minValueNode(node->right);
            node->data = temp->data;
            node->right = remove(node->right, temp->data->id);
        }
        return node;
    }

    TreeNode *minValueNode(TreeNode *node)
    {
        TreeNode *current = node;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
    }

    void displayMenu(string category )
    {
        inOrder(root, category);
    }

    FoodItem *getFoodItem(int id)
    {
        TreeNode *node = search(root, id);
        return node ? node->data : nullptr;
    }

    TreeNode *search(TreeNode *node, int id)
    {
        if (!node || node->data->id == id)
            return node;
        if (id < node->data->id)
            return search(node->left, id);
        return search(node->right, id);
    }
};
// Linked List Node for Order History
class OrderNode
{
public:
    string orderDetails;
    OrderNode *next;
    OrderNode(string details) : orderDetails(details), next(nullptr) {}
};

class OrderHistory
{
    OrderNode *head;

public:
    OrderHistory() : head(nullptr) {}

    void addOrder(string details)
    {
        OrderNode *newNode = new OrderNode(details);
        newNode->next = head;
        head = newNode;
    }

    void displayHistory()
    {
        OrderNode *temp = head;
        cout << "Order History:\n";
        while (temp)
        {
            cout << temp->orderDetails << endl;
            temp = temp->next;
        }
    }
};

// User Authentication Class
class UserAuth
{
private:
    unordered_map<string, string> users;
    const string filename = "user_data.txt";

    void loadUsersFromFile()
    {
        ifstream file(filename);
        if (!file)
        {
            return; // No file found, no users to load
        }
        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string username, password;
            getline(ss, username, ',');
            getline(ss, password);
            users[username] = password;
        }
        file.close();
    }

    void saveUsersToFile()
    {
        ofstream file(filename, ios::trunc);
        for (const auto &user : users)
        {
            file << user.first << "," << user.second << endl;
        }
        file.close();
    }

public:
    UserAuth()
    {
        loadUsersFromFile();
    }

    bool signup(string username, string password)
    {
        if (users.find(username) != users.end())
        {
            cout << "Username already exists. Please choose a different username.\n";
            return false;
        }
        users[username] = password;
        saveUsersToFile();
        cout << "Signup successful!\n";
        return true;
    }

    bool login(string username, string password)
    {
        if (users.find(username) == users.end())
        {
            cout << "Username not found.\n";
            return false;
        }
        if (users[username] == password)
        {
            cout << "Login successful!\n";
            return true;
        }
        else
        {
            cout << "Incorrect password.\n";
            return false;
        }
    }

};


// Graph Class for Delivery Locations
class Graph {
private:
    unordered_map<string, vector<pair<string, int>>> adjList; // Adjacency list with weights

public:
    void addEdge(const string &source, const string &destination, int weight) {
        adjList[source].emplace_back(destination, weight);
        adjList[destination].emplace_back(source, weight); // For undirected graph
    }

    vector<string> shortestPath(const string &start, const string &end) {
        unordered_map<string, int> distances;
        unordered_map<string, string> previous;
        for (auto &node : adjList) {
            distances[node.first] = INT_MAX;
        }
        distances[start] = 0;

        auto compare = [&distances](const string &a, const string &b) {
            return distances[a] > distances[b];
        };
        priority_queue<string, vector<string>, decltype(compare)> pq(compare);
        pq.push(start);

        while (!pq.empty()) {
            string current = pq.top();
            pq.pop();

            if (current == end)
                break;

            for (auto &neighbor : adjList[current]) {
                string nextNode = neighbor.first;
                int weight = neighbor.second;
                int newDist = distances[current] + weight;

                if (newDist < distances[nextNode]) {
                    distances[nextNode] = newDist;
                    previous[nextNode] = current;
                    pq.push(nextNode);
                }
            }
        }

        vector<string> path;
        for (string at = end; !at.empty(); at = previous[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());
        return path;
    }

    void displayLocations() {
        cout << "Available Delivery Locations:\n";
        for (auto &node : adjList) {
            cout << node.first << endl;
        }
    }
};

// Main Program
int main()
{
    UserAuth auth;
    string username, password;
    bool loggedIn = false;
    bool isAdmin = false;
     Graph deliveryGraph;

    // Add delivery locations and distances (example data)
    deliveryGraph.addEdge("Restaurant", "FCSE", 2);
    deliveryGraph.addEdge("Restaurant", "FME", 4);
    deliveryGraph.addEdge("Restaurant", "FES", 6);
    deliveryGraph.addEdge("FCSE", "FME", 1);
    deliveryGraph.addEdge("FME", "FES", 2);
    deliveryGraph.addEdge("FES", "Brabers Building", 3);
    deliveryGraph.addEdge("Brabers Building", "AcB", 5);

    MenuTree menu;
    string menuFilePath = "C:\\Users\\nazir\\OneDrive\\Desktop\\Dine_now\\menu.txt";
    menu.loadFromFile(menuFilePath);

    // Add a sample menu if file is empty or not found
    if (menu.getFoodItem(1) == nullptr) // Example: Check if the menu is empty
    {
        menu.addFoodItem(new FoodItem(1, "Burger", 8.99, "Desi"));
        menu.addFoodItem(new FoodItem(2, "Pasta", 5.49, "Continental"));
        menu.addFoodItem(new FoodItem(3, "Pizza", 7.99, "Continental"));
        menu.addFoodItem(new FoodItem(4, "Fried Rice", 6.99, "Chinese"));
        menu.addFoodItem(new FoodItem(5, "Biryani", 9.99, "Desi"));
        menu.saveToFile(menuFilePath); // Save initial menu
    }
    // User login/signup process
    while (!loggedIn)
    {
        int choice;
        cout << "\n1. Login\n2. Signup\n3. Admin Panel\n4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Enter Username: ";
            cin >> username;
            cout << "Enter Password: ";
            cin >> password;
            loggedIn = auth.login(username, password);
            break;

        case 2:
            cout << "Enter Username: ";
            cin >> username;
            cout << "Enter Password: ";
            cin >> password;
            loggedIn = auth.signup(username, password);
            break;

        case 3:
            cout << "Enter Admin Password: ";
            cin >> password;

            if (password == "admin123")
            {
                isAdmin = true;
                cout << "Admin access granted.\n";

                int adminChoice;
                do
                {
                    cout << "\nAdmin Panel:\n";
                    cout << "1. View Menu\n2. Add Food Item\n3. Remove Food Item\n4. Exit Admin Panel\n";
                    cout << "Enter your choice: ";
                    cin >> adminChoice;

                    switch (adminChoice)
                    {
                    case 1:
                    {
                        string category;
                        cout << "Enter category (Chinese, Desi, Continental): ";
                        cin >> category;
                        menu.displayMenu(category);
                        break;
                    }

                    case 2:
                    {
                        int id;
                        string name, category;
                        double price;
                        cout << "Enter Food ID: ";
                        cin >> id;
                        cout << "Enter Food Name: ";
                        cin.ignore();
                        getline(cin, name);
                        cout << "Enter Food Price: ";
                        cin >> price;
                        cout << "Enter Category (Chinese, Desi, Continental): ";
                        cin >> category;
                        menu.addFoodItem(new FoodItem(id, name, price, category));
                        menu.saveToFile("C:\\Users\\nazir\\OneDrive\\Desktop\\Dine_now\\menu.txt");
                        cout << "Food item added.\n";
                        break;
                    }

                    case 3:
                    {
                        int id;
                        cout << "Enter Food ID to remove: ";
                        cin >> id;
                        menu.removeFoodItem(id);
                        menu.saveToFile("C:\\Users\\nazir\\OneDrive\\Desktop\\Dine_now\\menu.txt");   
                        cout << "Food item removed.\n";
                        break;
                    }

                    case 4:
                        isAdmin = false;
                        cout << "Exiting Admin Panel.\n";
                        break;

                    default:
                        cout << "Invalid choice.\n";
                    }
                } while (isAdmin);
            }
            else
            {
                cout << "Invalid password.\n";
            }
            break;

        case 4:
            cout << "Exiting...\n";
            return 0;

        default:
            cout << "Invalid choice.\n";
        }
    }

    // Initialize Cart (Stack)
    stack<FoodItem *> cart;

    // Initialize Order Queue
    queue<string> orderQueue;

    // Initialize Order History (Linked List)
    OrderHistory history;

    int choice;
    do
    {
        cout << "\nFood Ordering System:\n";
        cout << "1. View Menu\n2. Add to Cart\n3. Remove Last Item from Cart\n4. Place Order\n5. View Cart\n6. View Order Queue\n7. Complete an Order\n8. View Order History\n9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            string category;
            cout << "Enter category (Chinese, Desi, Continental) or leave empty for all: ";
            cin >> category;
            menu.displayMenu(category);
            break;
        }

        case 2:
        {
            int id;
            cout << "Enter Food ID to add to cart: ";
            cin >> id;
            FoodItem *item = menu.getFoodItem(id);
            if (item)
            {
                cart.push(item);
                cout << item->name << " added to the cart.\n";
            }
            else
            {
                cout << "Invalid ID.\n";
            }
            break;
        }

        case 3:
            if (!cart.empty())
            {
                cout << "Removed " << cart.top()->name << " from the cart.\n";
                cart.pop();
            }
            else
            {
                cout << "Cart is empty.\n";
            }
            break;

case 4:
{
    cout << "Delivery Locations:\n";
    deliveryGraph.displayLocations();
    cout << "Enter your delivery location: ";
    string location;
    cin >> location;

    vector<string> path = deliveryGraph.shortestPath("Restaurant", location);
    if (path.size() > 1) {
        cout << "Shortest delivery path: ";
        for (const string &node : path) {
            cout << node << (node == location ? "\n" : " -> ");
        }
    } else {
        cout << "Invalid location or no path found.\n";
    }

   if (!cart.empty())
    {
        double totalAmount = 0;
        string orderDetails = "Order: ";
        stack<FoodItem *> tempCart = cart;
        // Calculate total amount of items in the cart
        while (!tempCart.empty())
        {
            orderDetails += tempCart.top()->name + ", ";
            totalAmount += tempCart.top()->price;  // Add item price to total
            tempCart.pop();
        }
        orderQueue.push(orderDetails);
        history.addOrder(orderDetails);
        cout << "Order placed!\n";
        cout << "Total amount for your order: $" << totalAmount << endl;
        
        // Clear the cart after placing the order
        while (!cart.empty()) {
            cart.pop();
        }
    }
    else
    {
        cout << "Cart is empty.\n";
    }
    break;
}


        case 5:
            if (!cart.empty())
            {
                stack<FoodItem *> tempCart = cart;
                cout << "Cart:\n";
                while (!tempCart.empty())
                {
                    cout << tempCart.top()->name << " - $" << tempCart.top()->price << endl;
                    tempCart.pop();
                }
            }
            else
            {
                cout << "Cart is empty.\n";
            }
            break;

        case 6:
            if (!orderQueue.empty())
            {
                queue<string> tempQueue = orderQueue;
                cout << "Order Queue:\n";
                while (!tempQueue.empty())
                {
                    cout << tempQueue.front() << endl;
                    tempQueue.pop();
                }
            }
            else
            {
                cout << "No pending orders.\n";
            }
            break;

        case 7:
            if (!orderQueue.empty())
            {
                cout << "Completed: " << orderQueue.front() << endl;
                orderQueue.pop();
            }
            else
            {
                cout << "No orders to complete.\n";
            }
            break;

        case 8:
        
            history.displayHistory();
            break;

 case 9:
{
    cout << "Exiting...\n";
    break;
}

        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 9);

    return 0;
}