#include <iostream> //Including all of my headings that will be used throughout my code
#include <string>     //iomanip for headings,limits for input validation, fstream for file writing, etc
#include <iomanip>
#include <limits>
#include <fstream>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class Client;
class Invoice;
class CourtDate; // Declaring all of my classes that  will be used in my program which includes Client,Invoice and CourtDate

class Court { // Declaring my Court Class in which I include 3 functions being Adding Court Dates, Removing Court Dates and Displaying my calendar
public:
    vector<CourtDate> courtDates; // Using a vector for storage

    void AddCourtDate();
    void RemoveCourtDate();
    void DisplayCalendar();
};

class CourtDate { // Listing my members of my CourtDate class which are date,court name and time of the court date which will be saved into a text file.
public:
    string date;
    string court_name;
    int time;

    CourtDate(const string& date, const string& court_name, int time)
        : date(date), court_name(court_name), time(time) {}
};

class Clients {
public:
    string name; // Listing all the members of my Clients Class which will include my clients name, their reference, their invoice total when generating their invoices and details.
    string reference;
    double invoice_total = 0; // invoice total has been initialised to 0 to ensure that the correct total is calculated
    vector<string> work_done;  // Use vector for size
    vector<double> work_prices;
    vector<double> payments_received;
    string phone;
    string email;

    void GenerateInvoice(); // Functions of generating invoice and Generating documents
    void GenerateDocument(const string& docType);
};

class Invoice {
public:
    double total; // Invoice class which includes total and calling the AddWork Function

    Invoice() : total(0) {}
    void AddWork(Clients& client, const string& description, double price);
};

bool Login(const string& username, const string& password) { // Login function to allow access to the main menu
    // Using a template to create my user names along with the valid pass keys
    map<string, string> users = {
        {"pa@valaw.co.za", "VALAW123!"},
        {"admin@valaw.co.za", "VALAW123!"},
        {"Joe@valaw.co.za", "VALAW123!"}
    };

    // Check if the username is valid  and the password matches whoch will allow the login
    return (users.count(username) > 0 && users[username] == password);
}


void Court::AddCourtDate() {
    string date, court_name; // Function to add court dates
    int time; // variables of the court date function which will be written to the file

    cout << "Enter the date of the court appearance (DD/MM/YYYY): "; // Prompt to enter the date of the court appearance
    cin.ignore(); // Using the ignore function to clear the buffer
    getline(cin, date); // Inputting the date into the date varaible

    cout << "Enter the court you will be attending: "; // Prompt to enter the court for the appearance
    getline(cin, court_name); // Inputting the data into the court name varaible

    cout << "Enter the time of the appearance (1 - 24): ";
    cin >> time; // Prompt and inputting the time (Using the 24h00 format therefor 15 = 3:00 pm)

    // Input validation for the time
    while (cin.fail() || time < 1 || time > 24) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid time. Please enter a valid time (1 - 24): ";
        cin >> time;
    }
        // Creating a new object
     CourtDate newCourtDate(date, court_name, time);

    // Add the new CourtDate object to the courtDates vector
    courtDates.push_back(newCourtDate);

    // Append the new court date information to the file
    ofstream courtFile("CourtDates.txt", ios::app);
    if (courtFile.is_open()) {
        courtFile << "Date: " << date << "\n"; // Writing the date, court and time of the court appearance to the file court_dates.txt
        courtFile << "Court: " << court_name << "\n";
        courtFile << "Time: " << time << "\n";
        courtFile << "---------------------------\n";
        courtFile.close();
        cout << "Court date added and saved to file successfully!\n"; // Message prompt to show that the date was saved successfully
    } else {
        cout << "Unable to open file to save court date." << endl;
    }
}

void Court::RemoveCourtDate() {
    if (courtDates.empty()) {
        cout << "No court dates available to remove." << endl;
        return;
    }

    string removalDate;
    cout << "Enter the date you would like to remove (DD/MM/YYYY): ";
    cin.ignore();
    getline(cin, removalDate);

    bool dateFound = false;

    // Loop through the vector to find and remove the matching court date
    for (size_t i = 0; i < courtDates.size(); ++i) {
        if (courtDates[i].date == removalDate) {
            cout << "Removing court date: " << courtDates[i].date << " at " << courtDates[i].court_name << " at " << courtDates[i].time << ":00" << endl;

            // Remove the court date by shifting the elements
            courtDates[i] = courtDates.back(); // Move the last element into the current position
            courtDates.pop_back(); // Removing the last element

            dateFound = true;
            cout << "Court date removed successfully." << endl;
            break; // Exit the loop after removing
        }
    }



    // Rewrite the CourtDates.txt file with the updated court dates
    ofstream courtFile("CourtDates.txt");
    if (courtFile.is_open()) {
        for (const auto& courtDate : courtDates) {
            courtFile << "Date: " << courtDate.date << "\n";
            courtFile << "Court: " << courtDate.court_name << "\n";
            courtFile << "Time: " << courtDate.time << "\n";
            courtFile << "---------------------------\n";
        }
        courtFile.close();
        cout << "Court date removed and file updated successfully!" << endl;
    } else {
        cout << "Unable to open file to update court dates." << endl;
    }
}

void Court::DisplayCalendar() {
    if (courtDates.empty()) {
        cout << "No court dates scheduled." << endl;
        return;
    } // Scanning through the court dates to view if it is empty

    cout << "\nScheduled Court Dates:\n";
    cout << left << setw(20) << "Date" << setw(20) << "Court" << setw(10) << "Time" << endl;
    cout << "-----------------------------------------\n";
    for (const auto& courtDate : courtDates) {
        cout << left << setw(20) << courtDate.date << setw(20) << courtDate.court_name << setw(10) << courtDate.time << endl;
    } // Printing all the court dates to with the court and time
}

void Clients::GenerateInvoice() {
    const string workOptions[6] = {
        "Upon attendance at court",
        "Upon appearance at court",
        "Upon drafting documentation",
        "Upon correspondences",
        "Upon consultation",
        "Custom work" // Having an array with options of common work executed by attorneys along with an array of prices
    };

    const double workPrices[5] = {1000, 1500, 1000, 500, 1000};

    int option = 0;
    double invoicetotal = 0;
    string date;

    cout << "\nEnter the reference of the client whom you would like to generate an invoice for: ";
    cin.ignore();
    getline(cin, reference); // Prompt to enter the reference for the invoice generation

    cout << "\nPrice guidelines for invoices\n";
    for (int i = 0; i < 5; i++) {
        cout << i + 1 << ". " << workOptions[i] << " - R" << workPrices[i] << endl;
    }
    cout << "6. Enter a different work type\n";
    cout << "7. Add a payment\n"; // Printing the invoice and the prices
    cout << "8. Exit\n";

    while (option != 8) {
        cout << "\nEnter an option (1-8) to add to the invoice: ";
        cin >> option;
        cin.ignore();

        if (option >= 1 && option <= 6) {
            cout << "\nEnter the date of the work done (DD/MM/YYYY): ";
            getline(cin, date);

            if (option == 6) {
                cout << "\nEnter the custom amount for the work type: ";
                double customAmount;
                cin >> customAmount; // Prompt to enter the custom work and the details of it
                cin.ignore();
                work_done.push_back(workOptions[5] + " on " + date);
                work_prices.push_back(customAmount);
                invoicetotal += customAmount;
            } else {
                work_done.push_back(workOptions[option - 1] + " on " + date);
                work_prices.push_back(workPrices[option - 1]);
                invoicetotal += workPrices[option - 1];
            }

        } else if (option == 7) {
            double payment = 0;
            cout << "\nEnter the payment amount: ";
            cin >> payment;
            cin.ignore();

            payments_received.push_back(payment);
            invoicetotal -= payment;
        } else if (option == 8) {
            cout << "\nExiting invoice generation...\n";
        } else {
            cout << "\nInvalid option, please try again.\n";
        }
    }

    // Displaying invoice details
    cout << "\n================= INVOICE DETAILS =================\n";
    cout << left << setw(30) << "Description of Work" << setw(30) << "Price(R)" << endl;
    cout << "---------------------------------------------------\n";
    for (size_t i = 0; i < work_done.size(); i++) {
        cout << left << setw(30) << work_done[i] << setw(45) << work_prices[i] << endl;
    }
    cout << "---------------------------------------------------\n";
    cout << left << setw(30) << "Total Amount Due" << setw(10) << invoicetotal << endl;
    cout << "===================================================\n";

    invoice_total += invoicetotal;
}

void Clients::GenerateDocument(const string& docType) {
    string filename = docType + "_" + reference + ".txt"; // Create a filename based on the document type and client reference
    ofstream documentFile(filename);

    if (!documentFile.is_open()) {
        cout << "Failed to create document." << endl;
        return;
    }

    // Writing document content based on the document type
    if (docType == "NoticeOfWithdrawal") {
        documentFile << "Notice of Withdrawal\n";
        documentFile << "Client: " << name << "\n";
        documentFile << "Reference: " << reference << "\n";
        documentFile << "This notice serves as a formal withdrawal from the case.\n";
    }

        else if (docType == "LetterOfDemand") {
        documentFile << "Letter of Demand\n";
        documentFile << "Client: " << name << "\n";
        documentFile << "Reference: " << reference << "\n";
        documentFile << "This letter demands payment for outstanding invoices.\n";
    }

        else if (docType == "ANCContract") {
        documentFile << "ANC Contract\n";
        documentFile << "Client: " << name << "\n";
        documentFile << "Reference: " << reference << "\n";
        documentFile << "This contract outlines the agreement between the client and the attorney.\n";
    }
        else {
        cout << "Invalid document type requested." << endl;
    }

    documentFile.close();
    cout << docType << " generated successfully: " << filename << endl;
}

void AddClient(shared_ptr<Clients> clientArray[], int& clientCounter) {
    shared_ptr<Clients> newClient = make_shared<Clients>();
    cout << "Enter client name: ";
    cin.ignore();
    getline(cin, newClient->name);

    cout << "Enter client reference: ";
    getline(cin, newClient->reference); // Adding client, prompting to enter the name reference, phone number and email for the clients

    cout << "Enter client phone number: ";
    getline(cin, newClient->phone);

    cout << "Enter client email: ";
    getline(cin, newClient->email);

    clientArray[clientCounter++] = newClient; // Add new client to the array
    cout << "Client added successfully.\n";
}

void DisplayClients(shared_ptr<Clients> clientArray[], int clientCounter) {
    if (clientCounter == 0) {
        cout << "No clients to display." << endl;
        return;
    }

    cout << "\nList of Clients:\n";
    for (int i = 0; i < clientCounter; ++i) {
        cout << "Name: " << clientArray[i]->name
             << ", Reference: " << clientArray[i]->reference
             << ", Phone: " << clientArray[i]->phone
             << ", Email: " << clientArray[i]->email << endl; // Displaying the clients with their name, reference, phone and email
    }
}

void RemoveClient(shared_ptr<Clients> clientArray[], int& clientCounter) {
    if (clientCounter == 0) {
        cout << "No clients to remove." << endl;
        return;
    }

    string reference;
    cout << "Enter client reference to remove: ";
    cin.ignore();
    getline(cin, reference);

    bool clientFound = false;

    // Loop through the client array to find the matching reference
    for (int i = 0; i < clientCounter; ++i) {
        if (clientArray[i]->reference == reference) {
            // Client found, perform the removal
            cout << "Removing client: " << clientArray[i]->name << endl;

            // Move the last client into the current position
            clientArray[i] = clientArray[--clientCounter];

            // Clear the last spot
            clientArray[clientCounter] = nullptr;

            clientFound = true;
            cout << "Client removed successfully." << endl;
            break;
        }
    }

    if (clientFound == false) {
        cout << "Client not found." << endl;
    }
}


int main() {
    shared_ptr<Clients> clientArray[100]; // Dynamic client array with smart pointers
    int clientCounter = 0;
    int option = 0;
    Court court;

    // User login
    string username, password;
    cout << "Welcome to LawyerUp! Please enter your firm's details to login.\n";
    cout << "Username: ";
    getline(cin, username); // Prompting for entry of the user name and password of the firm
    cout << "Password: ";
    getline(cin, password);

    while ((username != "admin@valaw.co.za" || username != "pa@valaw.co.za" || "Joe@valaw.co.za") && password != "VALAW123!") {
        cout << "Incorrect login details.\n";
        cout << "Username: ";
        getline(cin, username); // Prompting for entry of the user name and password of the firm
        cout << "Password: ";
        getline(cin, password);

        // Displaying a message informing the user the incorrect details have been entered
    }

    while (true) {
        cout << "\nMain Menu:\n";
        cout << "1. Add Client\n"; // While the details are correct displaying the main menu and showing all the options
        cout << "2. Remove Client\n"; // The main menu includes adding, removing, displaying clients as well as generating invoices
        cout << "3. Display Clients\n"; // , handling court dates and generating doucments.
        cout << "4. Generate Invoice\n";
        cout << "5. Add Court Date\n";
        cout << "6. Remove Court Date\n";
        cout << "7. Display Calendar\n";
        cout << "8. Generate Document\n";
        cout << "9. Exit\n";
        cout << "Enter an option (1-9): ";
        cin >> option;

        switch (option) {
            case 1:
                AddClient(clientArray, clientCounter);
                break;  // Using a switch statement to call the selected statement
            case 2:
                RemoveClient(clientArray, clientCounter);
                break;
            case 3:
                DisplayClients(clientArray, clientCounter);
                break;
            case 4:
                if (clientCounter > 0) {
                    clientArray[0]->GenerateInvoice(); // Example: generating invoice for the first client
                } else {
                    cout << "No clients available to generate an invoice.\n";
                }
                break;
            case 5:
                court.AddCourtDate();
                break;
            case 6:
                court.RemoveCourtDate();
                break;
            case 7:
                court.DisplayCalendar();
                break;
            case 8: {
                string docType;
                cout << "Enter document type (NoticeOfWithdrawal, LetterOfDemand, ANCContract): ";
                cin >> docType;
                if (clientCounter > 0) {
                    clientArray[0]->GenerateDocument(docType); // Example: generating document for the first client
                } else {
                    cout << "No clients available to generate documents.\n";
                }
                break;
            }
            case 9:
                cout << "Exiting program...\n";
                return 0;
            default:
                cout << "Invalid option, please try again.\n";
        }
    }

    return 0;
}
