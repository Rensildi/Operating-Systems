#include <stdio.h>

#define NUM_FRAMES 4
#define NUM_PAGES 8
#define MAX_AGE 255

// Structure for the page table entry
typedef struct {
    int page_number; // Page # in memory
    int age; // page age
} PageTableEntry;

int main() {
    // The sequence of page references
    int page_references[] = {4, 1, 5, 6, 2, 1, 2, 7, 6, 3, 2, 1};
    int num_references = sizeof(page_references) / sizeof(page_references[0]);

    // initializing page table
    // -1 = empty
    PageTableEntry page_table[NUM_FRAMES] = {{-1, 0}, {-1, 0}, {-1, 0}, {-1,0}};
    int clock_tick = 1; // tick start = 1
    int hits = 0;
    int misses = 0;

    // processing each page reference
    for (int i = 0; i < num_references; i++) {
        int page = page_references[i];
        int hit = 0;

        // Age all pages
        for (int j = 0; j < NUM_FRAMES; j++) {
            if (page_table[j].page_number != -1) {
                page_table[j].age /= 2; // shifting the age
            }
        }

        // Checking if the page is already in memory (hit or miss?)
        for (int j = 0; j < NUM_FRAMES; j++) {
            if (page_table[j].page_number == page) {
                // hit
                hit = 1;
                page_table[j].age = MAX_AGE; // reseting the age of the accesssing page
                hits++;
                break;
            }
        }
        
        // if miss
        if (!hit) {
            // find the page to replace (page with the lowest age)
            int min_age = MAX_AGE + 1;
            int replace_index = -1;
            for (int j = 0; j < NUM_FRAMES; j++) {
                if (page_table[j].age < min_age) {
                    min_age = page_table[j].age;
                    replace_index = j;
                }
            }

            // Replacing the page in the selected frame
            printf("Page %d accessed (Miss, replaced page %d), Clock Tick: %d\n", page, page_table[replace_index].page_number, clock_tick);
            page_table[replace_index].page_number = page;
            page_table[replace_index].age = MAX_AGE;

            misses;
        }

        // outputing the page table
        printf("Page Table at tick %d:\n", clock_tick);
        for (int j = 0; j < NUM_FRAMES; j++) {
            printf("Frame %d: Page %d\n", j, page_table[j].page_number);
        }

        clock_tick++;
    }

    // Outputing the total hits and misses
    printf("Total number of hits: %d\n", hits);
    printf("Total number of misses: %d\n", misses);

    return 0;
}