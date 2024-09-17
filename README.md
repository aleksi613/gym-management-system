# Gym Management System

## Overview
The **Gym Management System** is a console-based application designed to manage gym members, equipment, and generate reports. It features robust member and equipment management capabilities, offering a comprehensive efficient solution for gym operations. The program is written in C and supports functionalities like adding, deleting, updating members, tracking equipment statuses, and generating reports on gym equipment.

## Features
1. **Member Management**
   - Add new members with details like first and last name, phone number, gender, emergency contact information, and date of birth (minimum of 13 years old based on the real-time present date).
   - Search for members using multiple criteria: Member ID, First Name, Last Name, or both.
   - Update or delete member information.
   - Ensure the capacity to dynamically grow as the number of members increases.

2. **Equipment Management**
   - Add new gym equipment with functionality to track the number of functional and broken items.
   - Update equipment status (Operational or Under Maintenance) and assign a repair ETA.
   - Delete equipment and dynamically manage the equipment list as the capacity grows.

3. **Reports**
   - Generate real-time reports summarizing gym equipment statuses.
   - The report includes the total number of equipment, the count of operational and broken equipment, and the date the report was generated.
   - Unoperational equipment will include the amount and estimated repair date in the generated report.

4. **File Storage**
   - Member and equipment data is persisted using files (`members.dat` and `equipment.dat`), ensuring that all data is saved and reloaded when the program is restarted.

## File Structure
- `members.dat`: Stores all member-related data.
- `equipment.dat`: Stores all equipment-related data.

## Future Improvements
- Add membership management features to handle membership types and statuses.
- Implement security and authentication to restrict access to only authorized users.
- Integrate a GUI for better user interaction.
- Add an option to ban members.
- Add an option for authorized, authenticated users to wipe the database completely.

## Status
This project is a **Work in Progress (WIP)**. Additional fixes and future improvements, including optimizations, will be added.
