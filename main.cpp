#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Notice {
public:
    string category;
    string title;
    string content;

    void display() {
        cout << "\n-----------------------------";
        cout<<"notice";
        cout << "\nCategory: " << category;
        cout << "\nTitle   : " << title;
        cout << "\nContent : " << content;
        cout << "\n-----------------------------\n";
    }
};

class NoticeBoard {
private:
    void displayCategories() {
        cout << "\nSelect Category:\n";
        cout << "1. Assignments\n";
        cout << "2. Exams\n";
        cout << "3. Events\n";
        cout << "Choice: ";
    }

    string getCategory(int choice) {
        switch(choice) {
            case 1: return "Assignments";
            case 2: return "Exams";
            case 3: return "Events";
            default: return "Others";
        }
    }

    int countNotices() {
        ifstream fin("notices.txt");
        if (!fin) return 0;
        
        int count = 0;
        string line;
        while (getline(fin, line)) {
            count += 1;
        }
        fin.close();
        return count / 3;  // Each notice has 3 lines (category, title, content)
    }

public:
    void addNotice() {
        Notice n;
        cin.ignore();
        
        displayCategories();
        int catChoice;// catchoice - catgory choice 
        cin >> catChoice;
        n.category = getCategory(catChoice);
        
        cin.ignore();
        cout << "Enter Notice Title: ";
        getline(cin, n.title);
        cout << "Enter Notice Content: ";
        getline(cin, n.content);

        ofstream fout("notices.txt", ios::app);// file notices is formed ... append -add content without deletiong the exixting content
        fout << n.category << endl 
             << n.title << endl 
             << n.content << endl;
        fout.close();
        cout << "\nNotice Added Successfully!\n";
    }

    void viewNotices() {
        ifstream fin("notices.txt");
        if (!fin) {// if file donot exist then 
            cout << "\nNo Notices Found!\n";
            return;
        }
        
        string category, title, content;
        cout << "\n ALL NOTICES \n";
        int noticeNum = 1;
        
        while (getline(fin, category) && getline(fin, title) && getline(fin, content)) {
            cout << "\n[Notice #" << noticeNum << "]";
            cout << "\nCategory: " << category;
            cout << "\nTitle   : " << title;
            cout << "\nContent : " << content;
            cout << "\n-----------------------------";
            noticeNum++;
        }
        fin.close();
    }

    void viewByCategory() {
        ifstream fin("notices.txt");
        if (!fin) {
            cout << "\nNo Notices Found!\n";
            return;
        }
        
        displayCategories();
        int catChoice;
        cin >> catChoice;
        string selectedCategory = getCategory(catChoice);
        
        string category, title, content;
        bool found = false;
        int noticeNum = 1;
        
        cout << "\n========== " << selectedCategory << " NOTICES ==========\n";
        while (getline(fin, category) && getline(fin, title) && getline(fin, content)) {
            if (category == selectedCategory) {
                cout << "\n[Notice #" << noticeNum << "]";
                cout << "\nTitle   : " << title;
                cout << "\nContent : " << content;
                cout << "\n-----------------------------";
                found = true;
                noticeNum++;
            }
        }
        if (!found) cout << "\nNo " << selectedCategory << " Notices Found!\n";
        fin.close();
    }

    void searchNotice() {
        string keyword;
        cin.ignore();
        cout << "\nEnter keyword to search: ";
        getline(cin, keyword);

        ifstream fin("notices.txt");
        string category, title, content;
        bool found = false;
        int noticeNum = 1;
        
        cout << "\n========== SEARCH RESULTS ==========\n";
        while (getline(fin, category) 
        && getline(fin, title) 
        && getline(fin, content))
         {
            if (title.find(keyword) != string::npos || content.find(keyword) != string::npos) {
                cout << "\n[Notice #" << noticeNum << "]";
                cout << "\nCategory: " << category;
                cout << "\nTitle   : " << title;
                cout << "\nContent : " << content;
                cout << "\n-----------------------------";
                found = true;
                noticeNum++;
            }
        }
        if (!found) cout << "\nNo matching notices found!\n";
        fin.close();
    }

    void modifyNotice() {
        int totalNotices = countNotices();
        if (totalNotices == 0) {
            cout << "\nNo Notices Found!\n";
            return;
        }

        viewNotices();
        int noticeNum;
        cin.ignore();
        cout << "\nEnter Notice Number to Modify: ";
        cin >> noticeNum;

        if (noticeNum < 1 || noticeNum > totalNotices) {
            cout << "\nInvalid Notice Number!\n";
            return;
        }

        // Read all notices except the one to modify
        ifstream fin("notices.txt");
        ofstream fout("temp.txt");
        
        string category, title, content;
        int currentNum = 1;
        string modCategory, modTitle, modContent;
        
        while (getline(fin, category) && getline(fin, title) && getline(fin, content)) {
            if (currentNum == noticeNum) {
                modCategory = category;
                modTitle = title;
                modContent = content;
            } else {
                fout << category << endl << title << endl << content << endl;
            }
            currentNum++;
        }
        fin.close();
        fout.close();

        // Show current notice
        cout << "\n--- CURRENT NOTICE ---\n";
        cout << "Category: " << modCategory << endl;
        cout << "Title   : " << modTitle << endl;
        cout << "Content : " << modContent << endl;

        // Get new values
        cin.ignore();
        cout << "Enter New Title (Press Enter to skip): ";
        string newTitle;
        getline(cin, newTitle);
        if (newTitle.empty()) newTitle = modTitle;

        cout << "Enter New Content (Press Enter to skip): ";
        string newContent;
        getline(cin, newContent);
        if (newContent.empty()) newContent = modContent;

        // Write back with modified notice
        ifstream fin2("temp.txt");
        ofstream fout2("notices.txt");
        
        while (getline(fin2, category) && getline(fin2, title) && getline(fin2, content)) {
            fout2 << category << endl << title << endl << content << endl;
        }
        fout2 << modCategory << endl << newTitle << endl << newContent << endl;
        
        fin2.close();
        fout2.close();
        remove("temp.txt");

        cout << "\nNotice Updated Successfully!\n";
    }

    void deleteSpecificNotice() {
        int totalNotices = countNotices();
        if (totalNotices == 0) {
            cout << "\nNo Notices to Delete!\n";
            return;
        }

        viewNotices();
        int noticeNum;
        cin.ignore();
        cout << "\nEnter Notice Number to Delete: ";
        cin >> noticeNum;

        if (noticeNum < 1 || noticeNum > totalNotices) {
            cout << "\nInvalid Notice Number!\n";
            return;
        }

        // Read file and skip the one to delete
        ifstream fin("notices.txt");
        ofstream fout("temp.txt");
        
        string category, title, content;
        int currentNum = 1;
        
        while (getline(fin, category) && getline(fin, title) && getline(fin, content)) {
            if (currentNum != noticeNum) {
                fout << category << endl << title << endl << content << endl;
            }
            currentNum++;
        }
        fin.close();
        fout.close();

        // Replace original file
        remove("notices.txt");
        rename("temp.txt", "notices.txt");

        cout << "\nNotice Deleted Successfully!\n";
    }

    void deleteAllNotices() {
        ofstream fout("notices.txt", ios::trunc);
        fout.close();
        cout << "\nAll Notices Deleted Successfully!\n";
    }
};

bool login() {
    string pass = "";
    cout << "\nEnter Admin Password: ";
    cin >> pass;
    return (pass == "admin123");
}

int main() {
    NoticeBoard board;
    int userType, choice;

    cout << "========== NOTICE BOARD SYSTEM ==========\n";
    cout << "1. Admin Login\n2. User View\nChoice: ";
    cin >> userType;

    if (userType == 1) {
        if (!login()) {
            cout << "Access Denied!\n";
            return 0;
        }
        cout << "\nAdmin Login Successful!\n";
        
        // Admin Menu
        do {
            cout << "\n--- ADMIN PANEL ---\n";
            cout << "1. Add Notice\n";
            cout << "2. View All Notices\n";
            cout << "3. Modify Notice\n";
            cout << "4. Delete Specific Notice\n";
            cout << "5. Delete All Notices\n";
            cout << "0. Exit\n";
            cout << "Choice: ";
            cin >> choice;
            
            switch(choice) {
                case 1:
                    board.addNotice();
                    break;
                case 2:
                    board.viewNotices();
                    break;
                case 3:
                    board.modifyNotice();
                    break;
                case 4:
                    board.deleteSpecificNotice();
                    break;
                case 5: {
                    char confirm;
                    cout << "Are you sure? (y/n): ";
                    cin >> confirm;
                    if (confirm == 'y' || confirm == 'Y') {
                        board.deleteAllNotices();
                    }
                    break;
                }
                case 0:
                    break;
                default:
                    cout << "\nInvalid Choice!\n";
            }
        } while (choice != 0);

    } else if (userType == 2) {
        // User Menu (Read-Only)
        do {
            cout << "\n--- USER NOTICE BOARD ---\n";
            cout << "1. View All Notices\n";
            cout << "2. View by Category\n";
            cout << "3. Search Notice\n";
            cout << "0. Exit\n";
            cout << "Choice: ";
            cin >> choice;
            
            switch(choice) {
                case 1:
                    board.viewNotices();
                    break;
                case 2:
                    board.viewByCategory();
                    break;
                case 3:
                    board.searchNotice();
                    break;
                case 0:
                    break;
                default:
                    cout << "\nInvalid Choice!\n";
            }
        } while (choice != 0);
    } else {
        cout << "\nInvalid Choice!\n";
    }

    cout << "\nThank you for using Notice Board System!\n";
    return 0;
}
