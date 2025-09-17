#include <stdio.h>
#include <string.h>

#define CODE_LEN     32
#define STATUS_LEN   32
#define MAX_COURSES 512

/* ----------------------- Variable Scope (for your report) -----------------------
   - Global variables (below) are accessible to all functions in this file.
   - Local variables (inside functions) exist only within that function’s scope.
   Pros of globals: convenient for shared state (e.g., in-memory course list).
   Cons: easier to create accidental coupling; keep scope as small as practical.
------------------------------------------------------------------------------ */

// Globals used across functions (as required by the brief)
const char *REGS_FILE = "regs.txt";
typedef struct {
    int  id;
    char courseCode[CODE_LEN];
    char status[STATUS_LEN];
} Course;

Course courses[MAX_COURSES];
int courseCount = 0;

/* ------------------------------- I/O helpers -------------------------------- */
static void flush_line(void) {
    int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
}

/* ------------------------------ Core functions ------------------------------ */
/* Load all courses from file into the global array. Returns number loaded. */
int loadCourses(void) {
    FILE *fp = fopen(REGS_FILE, "r");
    if (!fp) { courseCount = 0; return 0; }

    courseCount = 0;
    while (courseCount < MAX_COURSES &&
           fscanf(fp, "%d,%31[^,],%31[^\n]\n",
                  &courses[courseCount].id,
                  courses[courseCount].courseCode,
                  courses[courseCount].status) == 3) {
        courseCount++;
    }
    fclose(fp);
    return courseCount;
}

/* Save all courses from the global array to file. Returns number saved. */
int saveCourses(void) {
    FILE *fp = fopen(REGS_FILE, "w");
    if (!fp) { printf("Could not open '%s' for writing.\n", REGS_FILE); return 0; }

    for (int i = 0; i < courseCount; i++) {
        fprintf(fp, "%d,%s,%s\n",
                courses[i].id, courses[i].courseCode, courses[i].status);
    }
    fclose(fp);
    return courseCount;
}

/* Return index of course by id, or -1 if not found. */
int findCourseById(int id) {
    for (int i = 0; i < courseCount; i++) {
        if (courses[i].id == id) return i;
    }
    return -1;
}

/* Add a course (returns 1 if added, 0 if ID exists or full). */
int addCourse(int id, const char *code, const char *status) {
    if (courseCount >= MAX_COURSES) {
        printf("Course list is full.\n");
        return 0;
    }
    if (findCourseById(id) != -1) {
        printf("Koops, course exists\n"); // exact wording requested earlier in the brief
        return 0;
    }
    courses[courseCount].id = id;
    strncpy(courses[courseCount].courseCode, code, CODE_LEN - 1);
    courses[courseCount].courseCode[CODE_LEN - 1] = '\0';
    strncpy(courses[courseCount].status, status, STATUS_LEN - 1);
    courses[courseCount].status[STATUS_LEN - 1] = '\0';
    courseCount++;
    return 1;
}

/* Search (returns index if found, -1 if not). Also prints the record if found. */
int searchCourses(int id) {
    int idx = findCourseById(id);
    if (idx >= 0) {
        printf("Found: ID=%d, Code=%s, Status=%s\n",
               courses[idx].id, courses[idx].courseCode, courses[idx].status);
    } else {
        printf("Course with ID %d not found.\n", id);
    }
    return (idx >= 0) ? idx : -1;
}

/* Update by id: returns 1 if updated, 0 if not found. */
int updateCourse(int id, const char *newCode, const char *newStatus) {
    int idx = findCourseById(id);
    if (idx == -1) return 0;
    strncpy(courses[idx].courseCode, newCode, CODE_LEN - 1);
    courses[idx].courseCode[CODE_LEN - 1] = '\0';
    strncpy(courses[idx].status, newStatus, STATUS_LEN - 1);
    courses[idx].status[STATUS_LEN - 1] = '\0';
    return 1;
}

/* Delete by id: returns 1 if deleted, 0 if not found. */
int deleteCourse(int id) {
    int idx = findCourseById(id);
    if (idx == -1) return 0;
    for (int i = idx; i < courseCount - 1; i++) {
        courses[i] = courses[i + 1];
    }
    courseCount--;
    return 1;
}

/* Display all courses currently in memory. */
void displayCourses(void) {
    if (courseCount == 0) { printf("(no courses)\n"); return; }
    printf("ID | Code        | Status\n");
    printf("--------------------------\n");
    for (int i = 0; i < courseCount; i++) {
        printf("%-3d| %-11s | %s\n",
               courses[i].id, courses[i].courseCode, courses[i].status);
    }
}

/* ---------------------------------- Main ----------------------------------- */
int main(void) {
    // Load from file at startup (persistence)
    int loaded = loadCourses();
    printf("Loaded %d record(s) from '%s'.\n", loaded, REGS_FILE);

    while (1) {
        int choice;
        printf("\nCourse Registration System\n");
        printf("1. Add course\n");
        printf("2. Display all courses\n");
        printf("3. Search by ID\n");
        printf("4. Update course\n");
        printf("5. Delete course\n");
        printf("6. Save to file\n");
        printf("7. Exit (auto-save)\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) { printf("Invalid input.\n"); return 1; }
        flush_line();

        if (choice == 1) {
            int id; char code[CODE_LEN]; char status[STATUS_LEN];
            printf("Enter ID (integer): ");
            if (scanf("%d", &id) != 1) { printf("Invalid ID.\n"); flush_line(); continue; }
            flush_line();

            printf("Enter course code (no spaces): ");
            if (scanf("%31s", code) != 1) { printf("Invalid code.\n"); flush_line(); continue; }
            flush_line();

            printf("Enter status (no spaces, e.g., Active/Closed): ");
            if (scanf("%31s", status) != 1) { printf("Invalid status.\n"); flush_line(); continue; }
            flush_line();

            int ok = addCourse(id, code, status);
            if (ok) printf("Course added.\n");

        } else if (choice == 2) {
            displayCourses();

        } else if (choice == 3) {
            int id;
            printf("Enter ID to search: ");
            if (scanf("%d", &id) != 1) { printf("Invalid ID.\n"); flush_line(); continue; }
            flush_line();
            (void)searchCourses(id);

        } else if (choice == 4) {
            int id; char code[CODE_LEN]; char status[STATUS_LEN];
            printf("Enter ID to update: ");
            if (scanf("%d", &id) != 1) { printf("Invalid ID.\n"); flush_line(); continue; }
            flush_line();

            if (findCourseById(id) == -1) {
                printf("Course with ID %d not found.\n", id);
                continue;
            }

            printf("Enter new course code (no spaces): ");
            if (scanf("%31s", code) != 1) { printf("Invalid code.\n"); flush_line(); continue; }
            flush_line();

            printf("Enter new status (no spaces): ");
            if (scanf("%31s", status) != 1) { printf("Invalid status.\n"); flush_line(); continue; }
            flush_line();

            int ok = updateCourse(id, code, status);
            if (ok) printf("Course updated.\n");
            else   printf("Update failed.\n");

        } else if (choice == 5) {
            int id;
            printf("Enter ID to delete: ");
            if (scanf("%d", &id) != 1) { printf("Invalid ID.\n"); flush_line(); continue; }
            flush_line();

            int ok = deleteCourse(id);
            if (ok) {
                printf("Course deleted.\n");
                // Persist the deletion immediately (optional but nice)
                (void)saveCourses();
            } else {
                printf("Course with ID %d not found.\n", id);
            }

        } else if (choice == 6) {
            int saved = saveCourses();
            printf("Saved %d record(s) to '%s'.\n", saved, REGS_FILE);

        } else if (choice == 7) {
            int saved = saveCourses(); // auto-save on exit
            printf("Saved %d record(s). Goodbye!\n", saved);
            break;

        } else {
            printf("Invalid choice. Please select 1..7.\n");
        }
    }
    return 0;
}
