zl#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"
#include "file.h"
#include "populate.h"

// Function to list all contacts in the address book
void listContacts(AddressBook *addressBook)
{
    // Iterate through all contacts and print their details
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        printf("Name: %s\nMobile No: %s\nMail id: %s\n\n",
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }
}
// Initialize the address book
void initialize(AddressBook *addressBook)
{
    addressBook->contactCount = 0;     // Set contact count to 0
    loadContactsFromFile(addressBook); // Load contacts from file during initialization
}
// Load contacts from a file into the address book
void loadContactsFromFile(AddressBook *addressBook)
{
    FILE *fptr = fopen("contacts.txt", "r"); // Open file for reading
    if (fptr == NULL)                        // Check if file exists
    {
        printf("File does not exist");
        return;
    }
    int count;
    fscanf(fptr, "%d\n", &count);       // Read number of contacts
    addressBook->contactCount = count; // Set contact count
    for (int i = 0; i < count; i++)
    {
        // Read contact details from file
        fscanf(fptr, "%[^,],%[^,],%[^\n]\n",
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }
    fclose(fptr); // Close the file
}
// Save contacts to file and exit the program
void saveAndExit(AddressBook *addressBook)
{
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS);              // Exit the program
}

// Save contacts from the address book to a file
void saveContactsToFile(AddressBook *addressBook)
{
    FILE *fptr = fopen("contacts.txt", "w+"); // Open file for writing
    if (fptr == NULL)                         // Check if file opened successfully
    {
        printf("Failed to open file for writing.\n");
        return;
    }
    fprintf(fptr, "%d\n", addressBook->contactCount); // Write number of contacts

    // Write each contact's details to the file
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        fprintf(fptr, "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }
    fclose(fptr); // Close the file
    printf("File closed successfully. Contacts saved.\n");
}
// Create a new contact and add it to the address book
void createContact(AddressBook *addressBook)
{
    int flag = 1;
    while (flag)
    {
        printf("Enter the name:\n");
        scanf(" %[^\n]", addressBook->contacts[addressBook->contactCount].name);
        int length = strlen(addressBook->contacts[addressBook->contactCount].name);
        int validname = 1;
        // Validate the name to contain only alphabetic characters and spaces
        for (int i = 0; i < length; i++)
        {
            if (!isalpha(addressBook->contacts[addressBook->contactCount].name[i]) && addressBook->contacts[addressBook->contactCount].name[i] != ' ')
            {
                validname = 0; // Set flag to invalid if any character is not alphabetic or space
                break;         // Exit loop if invalid character is found
            }
        }
        if (!validname)
        {
            printf("Invalid name Please enter alphabetic characters only.\n");
        }
        else
        {
            flag = 0; // Exit loop if name is valid
        }
    }
    flag = 1; // Reset flag for phone number input validation
    while (flag)
    {
        printf("Enter the phone number:\n");
        scanf(" %[^\n]", addressBook->contacts[addressBook->contactCount].phone);
        int length = strlen(addressBook->contacts[addressBook->contactCount].phone);
        flag = 0;
        // Validate phone number to be exactly 10 digits
        if (length != 10)
        {
            printf("Invalid phone number. Please enter a valid 10-digit number.\n");
            flag = 1; // Retry if phone number is invalid
            continue;
        }
        for (int i = 0; i < length; i++)
        {
            if (!isdigit(addressBook->contacts[addressBook->contactCount].phone[i]))
            {
                // Print error message if phone number contains non-digit characters
                printf("Invalid phone number. Only digits are allowed.\n");
                flag = 1; // Retry if phone number contains non-digit characters
                break;
            }
        }
        // Check for duplicate phone numbers
        for (int i = 0; i < addressBook->contactCount; i++)
        {
            if (strcmp(addressBook->contacts[i].phone, addressBook->contacts[addressBook->contactCount].phone) == 0)
            {
                printf("The phone number is already there..please enter a different number");
                flag = 1; // Retry if phone number already exists
                break;    // Exit loop if duplicate phone number is found
            }
        }
    }
    flag = 1; // Reset flag for email input validation
    while (flag)
    {
        printf("Enter the E-mail :\n");
        scanf(" %[^\n]", addressBook->contacts[addressBook->contactCount].email);
        char *ptr = strstr(addressBook->contacts[addressBook->contactCount].email, "@");
        // Validate email format
        if (ptr != NULL)
        {
            if (isalpha(*(ptr + 1))) // Check if the character after '@' is alphabetic
            {
                if (strstr(ptr + 1, ".com")) // Check if email contains ".com"
                {
                    int duplicate = 0; // Flag to indicate if the email is a duplicate
                    // Check for duplicate email addresses
                    for (int i = 0; i < addressBook->contactCount; i++)
                    {
                        if (strcmp(addressBook->contacts[i].email, addressBook->contacts[addressBook->contactCount].email) == 0)
                        {
                            duplicate = 1; // Set flag to indicate a duplicate email
                            break;
                        }
                    }
                    if (duplicate)
                    {
                        printf("The email is already there. Please enter a different email.\n");
                    }
                    else
                    {
                        flag = 0; // Exit loop if email is valid and unique
                    }
                }
            }
            else
            {
                printf("Invalid E-mail. The character after '@' must be an alphabetic character.\n");
            }
        }

        else
        {
            printf("Invalid E-mail. Enter again.\n");
        }
    }
    addressBook->contactCount++; // Increment contact count after adding a new contact
}
// Search for a contact by name, mobile number, or email
void searchContact(AddressBook *addressBook)
{
    int searchChoice;
    char searchName[50];
    char searchmobile[11];
    char searchemail[20];
    int found = 0;     // Flag to indicate if a contact was found
    int namematch = 0; // Flag to indicate if there was a name match
    int count = 0;     // Counter for the number of matches found
    printf("Select search option:\n");
    printf("1. Search by Name\n");
    printf("2. Search by Mobile Number\n");
    printf("3. Search by Email\n");
    printf("Enter your choice: ");
    scanf("%d", &searchChoice);

    switch (searchChoice)
    {
    case 1:
        printf("Enter the name of the contact to search: ");
        scanf(" %[^\n]", searchName);
        getchar();
        // Search for contacts with the specified name
        for (int i = 0; i < addressBook->contactCount; i++)
        {
            if (strcmp(addressBook->contacts[i].name, searchName) == 0)
            {
                namematch = 1; // Indicate a name match was found
                count++;
            }
        }
        if (!namematch)
        {
            printf("\nContact with the name '%s' not found.\n", searchName);
            break;
        }
        // If exactly one match is found, display contact details
        if (count == 1)
        {
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].name, searchName) == 0)
                {
                    printf("\nContact found:\n");
                    printf("Name: %s\n", addressBook->contacts[i].name);
                    printf("Mobile No: %s\n", addressBook->contacts[i].phone);
                    printf("Mail id: %s\n", addressBook->contacts[i].email);
                    found = 1; // Indicate that a contact was found
                    break;
                }
            }
        }
        // If more than one match is found, display a list of matching contacts
        else if (count > 1)
        {
            printf("Multiple  same name found. Please enter the mobile number to confirm:\n");
            scanf("%s", searchmobile);

            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].name, searchName) == 0 && strcmp(addressBook->contacts[i].phone, searchmobile) == 0)
                {
                    printf("\nContact found:\n");
                    printf("Name: %s\n", addressBook->contacts[i].name);
                    printf("Mobile No: %s\n", addressBook->contacts[i].phone);
                    printf("Mail id: %s\n", addressBook->contacts[i].email);
                    found = 1; // Indicate that contacts were found
                    break;
                }
            }

            if (!found)
            {
                printf("\nContact with the mobile number '%s' not found.\n", searchmobile);
                break;
            }
        }
        break;
    case 2:        // Search for a contact with the specified mobile number
        found = 0; // Reset found flag
        printf("Enter the mobile number to search:\n");
        scanf(" %[^\n]", searchmobile);

        int validmobile = 1;
        if (strlen(searchmobile) != 10) // Validate mobile number to be exactly 10 digits
        {
            validmobile = 0;
        }

        for (int i = 0; i < 10; i++)
        {
            if (!isdigit(searchmobile[i]))
            {
                validmobile = 0;
                break;
            }
        }
        if (!validmobile)
        {
            printf("Invalid mobile number.It should be exactly 10 digits.\n");
            break;
        }
        // Search for a contact with the specified mobile number
        for (int i = 0; i < addressBook->contactCount; i++)
        {
            if (strcmp(addressBook->contacts[i].phone, searchmobile) == 0)
            {
                printf("\nContact found:\n");
                printf("Name: %s\n", addressBook->contacts[i].name);
                printf("Mobile No: %s\n", addressBook->contacts[i].phone);
                printf("Mail id: %s\n", addressBook->contacts[i].email);
                found = 1; // Indicate that a contact was found
                break;
            }
        }
        if (!found)
        {
            printf("\nContact with the mobile no  '%s' not found.\n", searchmobile);
        }
        break;
    case 3:        // Search by Email
        found = 0; // Reset found flag
        printf("Enter the mail to search:\n");
        scanf(" %[^\n]", searchemail);
        // Search for a contact with the specified email
        char *ptr = strstr(searchemail, "@"); // Validate email format
        int valid = 1;
        if (ptr != NULL)
        {
            if (isalpha(*(ptr + 1)) && strstr(ptr + 1, ".com"))
            {
                for (int i = 0; i < addressBook->contactCount; i++)
                {
                    if (strcmp(addressBook->contacts[i].email, searchemail) == 0)
                    {
                        printf("\nContact found:\n");
                        printf("Name: %s\n", addressBook->contacts[i].name);
                        printf("Mobile No: %s\n", addressBook->contacts[i].phone);
                        printf("Mail id: %s\n", addressBook->contacts[i].email);
                        found = 1; // Indicate that a contact was found
                        break;
                    }
                }
                if (!found)
                {
                    printf("\nContact with the mail id  '%s' not found.\n", searchemail);
                }
            }
            else
            {
                printf("Invalid email format. It must contain an '@' symbol and a valid domain.\n");
                valid = 0;
            }
        }
        else
        {
            printf("Invalid email format. It must contain an '@' symbol and a valid domain.\n");
            valid = 0;
        }
        if (!valid)
        {
            break;
        }
        break;

    default:
        printf("Invalid option to search\n");
        break;
    }
}

void editContact(AddressBook *addressBook)
{
    int option;
    char name[100];
    char mobile[25];
    char newname[100];
    char mail[20];
    char newmail[20];
    int found = 0;
    int contactFound = 0;
    printf("Enter the option to edit:\n1.Name:\n2.Mobile no:\n3.Email id:\n");
    scanf("%d", &option);
    getchar();
    switch (option)
    {
    case 1: // Editing the contact's name
        printf("Enter the Name to Edit:\n");
        scanf(" %[^\n]", name);
        // Validate the provided name (alphabetic characters only)
        int validname = 1;
        int length1 = strlen(name);
        for (int i = 0; i < length1; i++)
        {
            if (!isalpha(name[i]) && name[i] != ' ')
            {
                validname = 0;
                break;
            }
        }
        if (!validname)
        {
            printf("Invalid name.Please enter alphabetic characters only\n");
            return; // Exit if the name is invalid
        }
        // Check if the name exists in the address book
        int count = 0;
        for (int i = 0; i < addressBook->contactCount; i++)
        {
            if (strcmp(addressBook->contacts[i].name, name) == 0)
            {
                count++;
            }
        }
        if (count == 0)
        {
            printf("\nContact with the name '%s' not found.\n", name);
            return; // Exit if no contact with the given name is found
        }
        else if (count == 1)
        {
            // Edit the name if only one contact with the given name is found
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].name, name) == 0)
                {
                    printf("Enter the New Name:\n");
                    scanf(" %[^\n]", newname);
                    // Validate the new name (alphabetic characters only)
                    int validname = 1;
                    int length = strlen(newname);
                    for (int j = 0; j < length; j++)
                    {
                        if (!isalpha(newname[j]) && newname[j] != ' ')
                        {
                            validname = 0;
                            break;
                        }
                    }

                    if (!validname)
                    {
                        printf("Invalid name. Please enter alphabetic characters only.\n");
                        return; // Exit if the new name is invalid
                    }
                    // Update the contact's name
                    strcpy(addressBook->contacts[i].name, newname);
                    printf("Contact updated successfully.\n");
                    found = 1;
                    break;
                }
            }
        }
        else
        {
            // multiple contacts having the same name
            printf("Multiple contacts with the name '%s' found. Please enter the mobile number to confirm:\n", name);
            scanf("%s", mobile);
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].name, name) == 0 && strcmp(addressBook->contacts[i].phone, mobile) == 0)
                {
                    printf("Enter the new name:\n");
                    scanf(" %[^\n]", newname);
                    // Validate the new name (alphabetic characters only)
                    int validname = 1;
                    int length = strlen(newname);
                    for (int j = 0; j < length; j++)
                    {
                        if (!isalpha(newname[j]) && newname[j] != ' ')
                        {
                            validname = 0;
                            break;
                        }
                    }
                    if (!validname)
                    {
                        printf("Invalid name. Please enter alphabetic characters only.\n");
                         // Exit if the new name is invalid
                    }
                    // Update the contact's name
                    strcpy(addressBook->contacts[i].name, newname);
                    printf("Contact with mobile number '%s' updated successfully.\n", mobile);
                    contactFound = 1;
                    break;
                }
            }
            if (!contactFound)
            {
                printf("No contact with name '%s' and mobile number '%s' found.\n", name, mobile);
            }
        }

        break;
    case 2: // Editing the contact's mobile number
        printf("Enter the Mobile No to edit:\n");
        scanf(" %[^\n]", mobile);

        // Validate the provided mobile number (10 digits only)
        int valid = 1;
        int length = strlen(mobile);
        if (length != 10)
        {
            valid = 0;// Invalid if length is not 10
        }
        else
        {
            for (int i = 0; i < length; i++)
            {
                if (!isdigit(mobile[i]))
                {
                    valid = 0;
                    break; // Exit if the mobile number is invalid
                }
            }
        }
        if (!valid)
        {
            printf("Invalid mobile number. Must be a 10-digit number.\n");
            break;
        }
        // Check if the mobile number exists in the address book
        int numberfound = 0;
        for (int i = 0; i < addressBook->contactCount; i++)
        {
            if (strcmp(addressBook->contacts[i].phone, mobile) == 0)
            {
                numberfound = 1;
                break;
            }
        }

        if (!numberfound)
        {
            printf("Contact With the mobile number '%s' not found\n", mobile);
            break; // Exit if the mobile number is not found
        }
        // Prompt for the new mobile number
        char newmobile[15];
        printf("Enter the new mobile number to edit:\n");
        scanf(" %[^\n]", newmobile);
        // Validate the new mobile number (10 digits only)
        valid = 1;
        length = strlen(newmobile);
        if (length != 10)
        {
            valid = 0;
        }
        else
        {
            for (int i = 0; i < length; i++)
            {
                if (!isdigit(mobile[i]))// Check if each character is a digit
                {
                    valid = 0;
                    break;
                }
            }
        }
        if (!valid)
        {
            printf("Invalid mobile number. Must be a 10-digit number.\n");
            break; // Exit if the new mobile number is invalid
        }

        // Check if the new mobile number already exists
        int newnumber = 0;
        for (int i = 0; i < addressBook->contactCount; i++)
        {
            if (strcmp(addressBook->contacts[i].phone, newmobile) == 0)
            {
                newnumber = 1;
                break;
            }
        }
        if (newnumber)
        {
            printf("The new mobile number '%s' is already there", newmobile);
            break; // Exit if the new mobile number already exists
        }
        // Update the contact's mobile number
        for (int i = 0; i < addressBook->contactCount; i++)
        {
            if (strcmp(addressBook->contacts[i].phone, mobile) == 0)
            {
                strcpy(addressBook->contacts[i].phone, newmobile);
                printf("Mobile number updated successfully");
                break;
            }
        }
        break;

    case 3: // Editing the contact's email address
        printf("Enter the Email To edit:\n");
        scanf(" %[^\n]", mail);

        // Check if the email exists in the address book
        int found = 0;
        for (int i = 0; i < addressBook->contactCount; i++)
        {
            if (strcmp(addressBook->contacts[i].email, mail) == 0)
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            printf("Email is not found '%s'\n", mail);
            break; // Exit if the email is not found
        }

        printf("Enter the new Email address:\n");
        scanf(" %[^\n]", newmail);
        // Validate the new email address
        char *ptr = strstr(newmail, "@");
        valid = 0;
        if (ptr != NULL)
        {
            // Check that there is an alphabetic character after '@' and ends with ".com"
           char*ptr1=strstr(ptr,".com");
           if (ptr1 != NULL && *(ptr1 + 4) == '\0' && ptr[1] != '\0' && isalpha(ptr[1]))
            {
                int duplicate = 0;
                // Check if the new email already exists
                for (int i = 0; i < addressBook->contactCount; i++)
                {
                    if (strcmp(addressBook->contacts[i].email, newmail) == 0)
                    {
                        duplicate = 1;
                        break;
                    }
                }
                if (duplicate)
                {
                    printf("The Email '%s' is already there\n", newmail);
                    valid = 0;
                }
                else
                {
                    // Update the contact's email address
                    for (int i = 0; i < addressBook->contactCount; i++)
                    {
                        if (strcmp(addressBook->contacts[i].email, mail) == 0)
                        {
                            strcpy(addressBook->contacts[i].email, newmail);
                            printf("Email updated successfully");
                            found = 1;
                            break;
                        }
                    }
                }
            }
            else
            {
                printf("Invalid email. The character after '@' must be an alphabetic character.\n");
                valid = 0;
            }
        }
        if (!valid)
        {
            break; // Exit if the email is invalid
        }
        break;

    default:
        break;
    }
}

void deleteContact(AddressBook *addressBook)
{
    int option;
    char name[50];
    char mobile[15];
    int found = 0;
    int count = 0;
    printf("Enter the Options To Delete:\n1.Name:\n");
    scanf("%d", &option);
    getchar();

    switch (option)
    {
    case 1: // Case to delete a contact by name
        printf("Enter the name to delete:\n");
        scanf(" %[^\n]", name);
        // Count the number of contacts with the given name
        for (int i = 0; i < addressBook->contactCount; i++)
        {
            if (strcmp(addressBook->contacts[i].name, name) == 0)
            {
                count++;
            }
        }
        // If no contact with the given name is found
        if (count == 0)
        {
            printf("Contact with the name '%s' not found.\n", name);
            return; // Exit if no contact is found
        }
        // If exactly one contact with the given name is found
        if (count == 1)
        {
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].name, name) == 0)
                {
                    // Shift all contacts after the one to be deleted one position left
                    for (int j = i; j < addressBook->contactCount; j++)
                    {
                        addressBook->contacts[j] = addressBook->contacts[j + 1];
                    }
                    addressBook->contactCount--; // Decrement the contact count
                    printf("Contact with the name '%s' has been deleted.\n", name);
                    found = 1; // Mark that a contact has been found and deleted
                    break;     // Exit the loop after deletion
                }
            }
        }
        // If multiple contacts with the same name are found
        else if (count > 1)
        {
            printf("Multiple contacts with the same name found. Please enter the mobile number to confirm:\n");
            scanf("%s", mobile);
            // Find the contact with the matching name and mobile number
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].name, name) == 0 && strcmp(addressBook->contacts[i].phone, mobile) == 0)
                {
                    // Shift all contacts after the one to be deleted one position left
                    for (int j = i; j < addressBook->contactCount; j++)
                    {
                        addressBook->contacts[j] = addressBook->contacts[j + 1];
                    }
                    addressBook->contactCount--; // Decrement the contact count
                    printf("Contact with the name '%s' and mobile '%s' has been deleted.\n", name, mobile);
                    found = 1; // Mark that a contact has been found and deleted
                    break;     // Exit the loop after deletion
                }
            } // If no contact with the specified mobile number is found
            if (!found)
            {
                printf("Contact with the mobile number '%s' not found.\n", mobile);
            }
        }

        break;
    default:
        printf("Invalid option");
        break;
    }
}
