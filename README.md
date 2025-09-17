# 📚 Course CSV Manager (C Project)

![C](https://img.shields.io/badge/language-C-blue)
![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![Last Commit](https://img.shields.io/github/last-commit/vhkarpuk/course-csv-manager-c)

A **course registration system in C** with CSV persistence.  
Users can add, search, update, delete, and list courses via a menu-driven interface.

---

## 📌 Features
- Add a course (ID, code, status)
- Display all courses in memory
- Search courses by ID
- Update or delete an existing course
- Save/load data to/from `regs.txt` (CSV file)
- Demonstrates **structs, arrays, file I/O, and persistence**

---

## 📸 Example Output (Screenshots)

**Menu**  
![Menu](assets/csv-menu.png)

**Add & Display**  
![Add Course](assets/csv-add.png)

**Update**  
![Update Course](assets/csv-update.png)

**Delete**  
![Delete Course](assets/csv-delete.png)

---

## 🛠️ Compilation & Usage

Make sure you have `gcc` installed.  
Clone the repository and compile the program:

```bash
gcc course-csv-manager-c.c -o course_manager
./course_manager
