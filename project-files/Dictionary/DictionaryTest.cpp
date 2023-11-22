// ----------------------------------------------------------------------------
// Shea Line
// sline
// 2023 Winter CSE101 pa8
// ----------------------------------------------------------------------------

#include<iostream>
#include<string>
#include<stdexcept>
#include"Dictionary.h"

using namespace std;

int main() {
    // Create a new Dictionary
    Dictionary dict;

    // Test setValue and size
    dict.setValue("apple", 1);
    dict.setValue("banana", 2);
    dict.setValue("cherry", 3);
    cout << "Size: " << dict.size() << endl;  // Should print: Size: 3

    // Test contains
    cout << "Contains 'banana': " << (dict.contains("banana") ? "true" : "false") << endl;  // Should print: Contains 'banana': true
    cout << "Contains 'date': " << (dict.contains("date") ? "true" : "false") << endl;  // Should print: Contains 'date': false

    // Test getValue
    cout << "Value of 'apple': " << dict.getValue("apple") << endl;  // Should print: Value of 'apple': 1

    // Test remove
    dict.remove("banana");
    cout << "Size after removing 'banana': " << dict.size() << endl;  // Should print: Size after removing 'banana': 2

    // Test iterator functions
    dict.begin();
    while (dict.hasCurrent()) {
        cout << "Current key: " << dict.currentKey() << ", Current value: " << dict.currentVal() << endl;
        dict.next();
    }

    // Test clear
    dict.clear();
    cout << "Size after clear: " << dict.size() << endl;  // Should print: Size after clear: 0

    // Test copy constructor
    dict.setValue("apple", 1);
    dict.setValue("banana", 2);
    Dictionary dict2(dict);
    cout << "Size of copied dictionary: " << dict2.size() << endl;  // Should print: Size of copied dictionary: 2

    // Test equals
    cout << "Are dict and dict2 equal? " << (dict.equals(dict2) ? "Yes" : "No") << endl;  // Should print: Are dict and dict2 equal? Yes

    // Test operator==
    cout << "Are dict and dict2 equal (using operator==)? " << (dict == dict2 ? "Yes" : "No") << endl;  // Should print: Are dict and dict2 equal (using operator==)? Yes

    // Test operator=
    Dictionary dict3;
    dict3 = dict;
    cout << "Size of dictionary assigned from dict: " << dict3.size() << endl;  // Should print: Size of dictionary assigned from dict: 2

    // Test operator<<
    cout << "Contents of dict: " << endl << dict;

    // Test to_string and pre_string
    cout << "In-order string representation of dict: " << endl << dict.to_string();
    cout << "Pre-order string representation of dict: " << endl << dict.pre_string();

    return 0;
}