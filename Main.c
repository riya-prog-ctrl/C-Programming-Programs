/*
 * THE INVISIBLE INFLUENCE DETECTOR
 * A menu-driven educational simulator on social and psychological influences.
 * Beginner-level C: functions, structs, arrays, loops, switch, file handling.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Constants ---------- */

#define MAX_RECORDS      1000
#define NUM_CATEGORIES   5
#define DATA_FILE        "influence_records.txt"
#define LINE_LENGTH      200

#define CAT_SOCIAL       1
#define CAT_FOMO         2
#define CAT_POPULARITY   3
#define CAT_AUTHORITY    4
#define CAT_EXPOSURE     5

#define EXPOSURE_COUNT   4
#define CHECKLIST_COUNT  6

/* ---------- Structures ---------- */

/* One completed decision, saved to file and used for reports */
struct DecisionRecord {
    int scenarioId;
    int category;
    int initialChoice;   /* 1 or 2 */
    int finalChoice;     /* 1 or 2 */
    int changed;         /* 1 if changed, 0 if kept */
};

/* A two-option scenario with a simulated influence */
struct Scenario {
    int         id;
    const char *title;
    const char *situation;
    const char *optionA;
    const char *optionB;
    const char *influence;
    const char *lesson;
};

/* A claim judged under two source descriptions */
struct AuthorityScenario {
    int         id;
    const char *title;
    const char *claim;
    const char *lesson;
};

/* A claim shown repeatedly in different contexts */
struct ExposureScenario {
    int         id;
    const char *title;
    const char *claim;
    const char *lesson;
};

/* ---------- Global data ---------- */

struct DecisionRecord records[MAX_RECORDS];
int recordCount = 0;

/* Scenario data (fictional products, people, and statistics only) */

struct Scenario socialScenarios[3] = {
    {101, "Choosing a Product",
     "You are choosing between two products.\n"
     "Product A costs Rs. 500 and has the features you need.\n"
     "Product B costs Rs. 1000 and is preferred by most people you know.",
     "Product A (Rs. 500)", "Product B (Rs. 1000)",
     "Simulated update: 70% of your group now prefers Product B.\n"
     "Your friends keep recommending it.",
     "Group preference can shift a choice even when the product facts stay the same."},

    {102, "Choosing a Study App",
     "You need an app to plan your study schedule.\n"
     "Option A is a free, simple app with the basic features.\n"
     "Option B is a paid app (Rs. 300 per month) with extra features.",
     "Free simple app", "Paid app (Rs. 300/month)",
     "Simulated update: Your closest friend strongly insists that the paid app\n"
     "is the only one worth using.",
     "A trusted person's opinion can feel like evidence, even when it is only one view."},

    {103, "Team Lunch Decision",
     "Your team is deciding where to eat lunch.\n"
     "Option A is Green Bowl: quieter, healthy food, a 10-minute walk.\n"
     "Option B is Spice Corner: popular, crowded, closer to the office.",
     "Green Bowl", "Spice Corner",
     "Simulated update: Almost everyone in the team has already voted for Spice Corner.",
     "Group expectations often shape small everyday choices without anyone saying so."}
};

struct Scenario fomoScenarios[3] = {
    {201, "Wireless Headphones",
     "You need new headphones. Both options cost Rs. 2000.\n"
     "Option A: Buy them now.\n"
     "Option B: Wait a week and compare reviews first.",
     "Buy now", "Wait and compare",
     "SIMULATED MESSAGE: Only two items left in stock!",
     "Scarcity messages are often designed to speed up decisions."},

    {202, "Online Course Offer",
     "An online course costs Rs. 1500 and you are interested in it.\n"
     "Option A: Enroll today.\n"
     "Option B: Think it over for a few days.",
     "Enroll today", "Think it over",
     "SIMULATED MESSAGE: This offer expires in five minutes!",
     "Deadlines that feel very close can stop you from checking the facts."},

    {203, "Smartwatch Purchase",
     "You are looking at a smartwatch. You do not urgently need it.\n"
     "Option A: Buy the smartwatch.\n"
     "Option B: Skip it for now.",
     "Buy the smartwatch", "Skip it for now",
     "SIMULATED MESSAGE: Thousands of people have already purchased this product!",
     "Other people's purchases do not tell you whether you need the product."}
};

struct Scenario popularityScenarios[3] = {
    {301, "Weekend Movie",
     "Pick a movie for Saturday night.\n"
     "Option A: 'Laugh Lane', a light comedy.\n"
     "Option B: 'Night Train', a thriller.",
     "Laugh Lane (comedy)", "Night Train (thriller)",
     "SIMULATED STATISTIC: In a poll, 85% of viewers chose Laugh Lane;\n"
     "only 15% chose Night Train.",
     "Popularity tells you what many people like, not what is best for you."},

    {302, "Quiz Question",
     "Question: How many bones does a typical adult human body have?\n"
     "Option A: 206\n"
     "Option B: 212",
     "206", "212",
     "SIMULATED STATISTIC: In a quiz, 85% of people answered 212.",
     "Popularity is not evidence of correctness. Check reliable sources."},

    {303, "Laptop Bag",
     "You need a bag for your laptop.\n"
     "Option A: A sturdy bag for Rs. 1200.\n"
     "Option B: A basic bag for Rs. 400.",
     "Sturdy bag (Rs. 1200)", "Basic bag (Rs. 400)",
     "SIMULATED STATISTIC: In a forum, 85% of reviewers recommend the sturdy bag.",
     "A majority opinion may reflect other needs, budgets, or tastes."}
};

struct AuthorityScenario authorityScenarios[3] = {
    {401, "Water Bottle Claim",
     "This water bottle keeps drinks cold for 24 hours.",
     "A claim should be judged by evidence, not by who makes it."},

    {402, "Study Planner Claim",
     "This planner app will double your study time.",
     "Titles such as 'expert' do not replace checking the evidence."},

    {403, "Earbuds Claim",
     "These earbuds block all outside noise completely.",
     "Credibility labels can make claims feel more trustworthy than they are."}
};

struct ExposureScenario exposureScenarios[3] = {
    {501, "Notebook Claim",
     "Brand Z notebooks never tear, even after a year of daily use.",
     "Seeing a claim often makes it feel familiar, but familiarity does not prove it is true."},

    {502, "Battery Claim",
     "Brand Q batteries last twice as long as any other brand.",
     "Repetition can build a feeling of truth without any new evidence."},

    {503, "Coffee Mug Claim",
     "The Glow mug keeps coffee hot for three hours.",
     "Checking a claim once and seeing it repeatedly are very different things."}
};

const char *exposureContexts[EXPOSURE_COUNT] = {
    "Seen on a shopping website banner",
    "Mentioned in a short video advertisement",
    "Appeared in a friend's shared post",
    "Printed on a store poster"
};

const char *checklistQuestions[CHECKLIST_COUNT] = {
    "Do I have enough information to make this decision?",
    "Am I choosing this because I genuinely want it?",
    "Would I make the same decision if nobody else knew about it?",
    "Am I reacting to urgency rather than evaluating the facts?",
    "Have I checked whether the information is reliable?",
    "Am I confusing popularity with correctness?"
};

/* ---------- Display helpers ---------- */

void printDivider(void)
{
    printf("========================================\n");
}

void displayMainMenu(void)
{
    printf("\n");
    printDivider();
    printf("       THE INVISIBLE INFLUENCE DETECTOR\n");
    printf(" Are your decisions really your own?\n");
    printDivider();
    printf("1. Social Pressure Simulator\n");
    printf("2. FOMO and Artificial Urgency\n");
    printf("3. Popularity Influence Simulator\n");
    printf("4. Authority and Persuasion Simulator\n");
    printf("5. Repeated Exposure Simulator\n");
    printf("6. Decision Awareness Report\n");
    printf("7. Independent Thinking Checklist\n");
    printf("8. View Previous Results\n");
    printf("9. Exit\n");
    printDivider();
}

const char *getCategoryName(int category)
{
    switch (category)
    {
        case CAT_SOCIAL:     return "Social pressure";
        case CAT_FOMO:       return "FOMO / urgency";
        case CAT_POPULARITY: return "Popularity influence";
        case CAT_AUTHORITY:  return "Authority / persuasion";
        case CAT_EXPOSURE:   return "Repeated exposure";
        default:             return "Unknown";
    }
}

void showOptions(const char *optionA, const char *optionB)
{
    printf("  1. %s\n", optionA);
    printf("  2. %s\n", optionB);
}

/* ---------- Input validation ---------- */

void clearInputBuffer(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        /* discard leftover characters */
    }
}

/* Keeps asking until the user enters a whole number from min to max */
int readIntInRange(const char *prompt, int min, int max)
{
    int value;
    int result;

    while (1)
    {
        printf("%s", prompt);
        result = scanf("%d", &value);

        if (result == EOF)
        {
            printf("\nInput closed. Goodbye!\n");
            exit(0);
        }

        if (result == 1 && value >= min && value <= max)
        {
            clearInputBuffer();
            return value;
        }

        printf("Invalid input. Please enter a number from %d to %d.\n", min, max);
        clearInputBuffer();
    }
}

/* Keeps asking until the user types Y or N. Returns 1 for Y, 0 for N */
int readYesNo(const char *prompt)
{
    char answer;

    while (1)
    {
        printf("%s", prompt);
        if (scanf(" %c", &answer) != 1)
        {
            printf("\nInput closed. Goodbye!\n");
            exit(0);
        }
        clearInputBuffer();

        if (answer == 'y' || answer == 'Y')
        {
            return 1;
        }
        if (answer == 'n' || answer == 'N')
        {
            return 0;
        }
        printf("Please type Y or N.\n");
    }
}

void pressEnterToContinue(void)
{
    printf("\nPress Enter to continue...");
    clearInputBuffer();
}

/* ---------- Independent Thinking Checklist ---------- */

/* Runs the six reflection questions. Does not pause; callers handle that */
void independentThinkingChecklist(void)
{
    int i;
    int yesCount = 0;

    printf("\n");
    printDivider();
    printf("     INDEPENDENT THINKING CHECKLIST\n");
    printDivider();
    printf("Answer Y or N. This is a reflection exercise only.\n\n");

    for (i = 0; i < CHECKLIST_COUNT; i++)
    {
        printf("Q%d. %s\n", i + 1, checklistQuestions[i]);
        if (readYesNo("    Your answer (Y/N): "))
        {
            yesCount++;
        }
    }

    printf("\nYou answered YES to %d of %d questions.\n", yesCount, CHECKLIST_COUNT);
    printf("These answers are for your reflection only. They do not prove\n");
    printf("whether your thinking was independent or influenced.\n");
}

/* Offered after some simulations so the user can reflect right away */
void offerChecklist(void)
{
    if (readYesNo("\nOpen the Independent Thinking Checklist now? (Y/N): "))
    {
        independentThinkingChecklist();
    }
}

/* ---------- Decision storage and analysis ---------- */

/* Appends one record to the data file so earlier records are never deleted */
void saveResults(struct DecisionRecord r)
{
    FILE *fp = fopen(DATA_FILE, "a");

    if (fp == NULL)
    {
        printf("Warning: could not save the result to file.\n");
        return;
    }

    fprintf(fp, "%d %d %d %d %d\n",
            r.scenarioId, r.category, r.initialChoice, r.finalChoice, r.changed);
    fclose(fp);
}

/*
 * Loads saved records when the program starts. Skips any line that is
 * malformed or holds out-of-range values, so bad data cannot break the report.
 * Returns the number of valid records loaded.
 */
int loadResults(void)
{
    FILE *fp = fopen(DATA_FILE, "r");
    char line[LINE_LENGTH];
    struct DecisionRecord r;
    int skipped = 0;
    int count = 0;
    int fileHasMore = 0;

    if (fp == NULL)
    {
        return 0;   /* no file yet: this is a first run */
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        int fields = sscanf(line, "%d %d %d %d %d",
                            &r.scenarioId, &r.category, &r.initialChoice,
                            &r.finalChoice, &r.changed);

        if (fields != 5 ||
            r.category < CAT_SOCIAL || r.category > CAT_EXPOSURE ||
            r.initialChoice < 1 || r.initialChoice > 3 ||
            r.finalChoice < 1 || r.finalChoice > 3)
        {
            skipped++;
            continue;
        }

        if (count >= MAX_RECORDS)
        {
            fileHasMore = 1;
            break;
        }

        records[count] = r;
        count++;
    }

    fclose(fp);
    recordCount = count;

    if (skipped > 0)
    {
        printf("Note: %d damaged line(s) in the data file were skipped.\n", skipped);
    }
    if (fileHasMore)
    {
        printf("Warning: more than %d records are in the file. Only the first %d are loaded.\n",
               MAX_RECORDS, MAX_RECORDS);
    }

    return count;
}

/* Stores a completed decision in memory and in the file */
void recordDecision(int scenarioId, int category, int initialChoice, int finalChoice)
{
    struct DecisionRecord r;

    if (recordCount >= MAX_RECORDS)
    {
        printf("Warning: record storage is full. This result was not saved.\n");
        return;
    }

    r.scenarioId     = scenarioId;
    r.category       = category;
    r.initialChoice  = initialChoice;
    r.finalChoice    = finalChoice;
    r.changed        = (initialChoice != finalChoice) ? 1 : 0;

    records[recordCount] = r;
    recordCount++;

    saveResults(r);
}

/* Prints the comparison and returns 1 if the choice changed, 0 if kept */
int compareDecisions(int initialChoice, int finalChoice,
                     const char *optionA, const char *optionB)
{
    const char *first = (initialChoice == 1) ? optionA : optionB;
    const char *last  = (finalChoice == 1)   ? optionA : optionB;

    printf("\n--- Your decisions ---\n");
    printf("Initial decision: %s\n", first);
    printf("Final decision:   %s\n", last);

    if (initialChoice != finalChoice)
    {
        printf("Result: Your decision CHANGED after the new information.\n");
        printf("A changed decision is not automatically wrong. The new information\n");
        printf("may have been useful, or it may have influenced you. Think about which.\n");
        return 1;
    }

    printf("Result: Your decision STAYED the same.\n");
    return 0;
}

/* Returns the percentage of changed decisions. Returns 0 if total is 0 */
float calculateChangePercentage(int changed, int total)
{
    if (total == 0)
    {
        return 0.0f;
    }
    return ((float)changed / (float)total) * 100.0f;
}

/* ---------- Simulation engine for two-option scenarios ---------- */

void runScenario(int category, struct Scenario s)
{
    int initialChoice;
    int finalChoice;

    printf("\n");
    printDivider();
    printf("%s\n", s.title);
    printDivider();
    printf("%s\n\n", s.situation);
    showOptions(s.optionA, s.optionB);
    initialChoice = readIntInRange("\nYour initial choice (1 or 2): ", 1, 2);

    printf("\n--- NEW INFORMATION (simulated) ---\n");
    printf("%s\n", s.influence);

    printf("\nPlease reconsider your decision:\n");
    showOptions(s.optionA, s.optionB);
    finalChoice = readIntInRange("\nYour final choice (1 or 2): ", 1, 2);

    recordDecision(s.id, category, initialChoice, finalChoice);
    compareDecisions(initialChoice, finalChoice, s.optionA, s.optionB);

    printf("\nLesson: %s\n", s.lesson);
}

/* ---------- Module A: Social Pressure ---------- */

void socialPressureSimulator(void)
{
    int i;

    printf("\nInstructions: Make a choice, then see new social information\n");
    printf("and decide again. Each choice is recorded for your report.\n");

    for (i = 0; i < 3; i++)
    {
        runScenario(CAT_SOCIAL, socialScenarios[i]);
    }

    offerChecklist();
    pressEnterToContinue();
}

/* ---------- Module B: FOMO and Artificial Urgency ---------- */

void fomoSimulator(void)
{
    int i;

    printf("\nInstructions: The messages in these scenarios are SIMULATED.\n");
    printf("They are not real offers.\n");

    for (i = 0; i < 3; i++)
    {
        runScenario(CAT_FOMO, fomoScenarios[i]);
    }

    printf("\nTip: Before buying anything under time pressure, pause and check the facts.\n");

    offerChecklist();
    pressEnterToContinue();
}

/* ---------- Module C: Popularity and Majority Influence ---------- */

void popularitySimulator(void)
{
    int i;

    printf("\nInstructions: The statistics shown are SIMULATED and fictional.\n");

    for (i = 0; i < 3; i++)
    {
        runScenario(CAT_POPULARITY, popularityScenarios[i]);
    }

    pressEnterToContinue();
}

/* ---------- Module D: Authority and Persuasion ---------- */

/*
 * The same claim is shown twice: first with an anonymous source,
 * then with a source described as an expert. Both sources are fictional.
 */
void runAuthorityScenario(struct AuthorityScenario s)
{
    int initialChoice;
    int finalChoice;

    printf("\n");
    printDivider();
    printf("%s\n", s.title);
    printDivider();

    printf("Source: An anonymous user with no details.\n");
    printf("Claim: \"%s\"\n", s.claim);
    printf("\n  1. I believe this claim\n");
    printf("  2. I doubt this claim\n");
    initialChoice = readIntInRange("\nYour response (1 or 2): ", 1, 2);

    printf("\n--- SAME CLAIM, NEW SOURCE (simulated) ---\n");
    printf("Source: A person presented as an expert in this product type.\n");
    printf("Claim: \"%s\"\n", s.claim);
    printf("\n  1. I believe this claim\n");
    printf("  2. I doubt this claim\n");
    finalChoice = readIntInRange("\nYour response (1 or 2): ", 1, 2);

    recordDecision(s.id, CAT_AUTHORITY, initialChoice, finalChoice);
    compareDecisions(initialChoice, finalChoice, "I believe", "I doubt");

    printf("\nLesson: %s\n", s.lesson);
}

void authoritySimulator(void)
{
    int i;

    printf("\nInstructions: Compare how you respond to the same claim\n");
    printf("when only the source description changes.\n");

    for (i = 0; i < 3; i++)
    {
        runAuthorityScenario(authorityScenarios[i]);
    }

    pressEnterToContinue();
}

/* ---------- Module E: Repeated Exposure and Familiarity ---------- */

void runExposureScenario(struct ExposureScenario s)
{
    int initialChoice;
    int finalChoice;
    int i;

    printf("\n");
    printDivider();
    printf("%s\n", s.title);
    printDivider();

    printf("Claim seen once: \"%s\"\n", s.claim);
    printf("\n  1. It seems true\n");
    printf("  2. I am not sure it is true\n");
    initialChoice = readIntInRange("\nYour response (1 or 2): ", 1, 2);

    printf("\n--- The same claim appears again (simulated exposures) ---\n");
    for (i = 0; i < EXPOSURE_COUNT; i++)
    {
        printf("Exposure %d: %s\n", i + 1, exposureContexts[i]);
        printf("   \"%s\"\n", s.claim);
    }

    printf("\nDoes the claim feel more familiar now?\n");
    printf("\n  1. It seems true\n");
    printf("  2. I am not sure it is true\n");
    finalChoice = readIntInRange("\nYour response (1 or 2): ", 1, 2);

    recordDecision(s.id, CAT_EXPOSURE, initialChoice, finalChoice);
    compareDecisions(initialChoice, finalChoice, "It seems true", "Not sure");

    printf("\nLesson: %s\n", s.lesson);
}

void repeatedExposureSimulator(void)
{
    int i;

    printf("\nInstructions: A fictional claim is shown once, then repeated\n");
    printf("in different places. Notice whether it feels more believable.\n");

    for (i = 0; i < 3; i++)
    {
        runExposureScenario(exposureScenarios[i]);
    }

    pressEnterToContinue();
}

/* ---------- Module F: Decision Awareness Report ---------- */

void generateAwarenessReport(void)
{
    int total = recordCount;
    int changed = 0;
    int categoryTotal[NUM_CATEGORIES + 1];
    int categoryChanged[NUM_CATEGORIES + 1];
    int i;
    int cat;
    int influenceNumber = 1;

    for (cat = 0; cat <= NUM_CATEGORIES; cat++)
    {
        categoryTotal[cat] = 0;
        categoryChanged[cat] = 0;
    }

    for (i = 0; i < recordCount; i++)
    {
        cat = records[i].category;
        categoryTotal[cat]++;
        if (records[i].changed)
        {
            changed++;
            categoryChanged[cat]++;
        }
    }

    printf("\n");
    printDivider();
    printf("       DECISION AWARENESS REPORT\n");
    printDivider();

    if (total == 0)
    {
        printf("No scenarios completed yet.\n");
        printf("Complete at least one simulation to generate a report.\n");
        printDivider();
        pressEnterToContinue();
        return;
    }

    printf("Scenarios completed: %d\n", total);
    printf("Initial decisions changed: %d\n", changed);
    printf("Decisions retained: %d\n", total - changed);
    printf("Decision change percentage: %.1f%%\n",
           calculateChangePercentage(changed, total));

    printf("\nChanges by influence type:\n");
    for (cat = 1; cat <= NUM_CATEGORIES; cat++)
    {
        if (categoryTotal[cat] > 0)
        {
            printf("  %-24s %d changed out of %d\n",
                   getCategoryName(cat), categoryChanged[cat], categoryTotal[cat]);
        }
    }

    printf("\nInfluences encountered:\n");
    for (cat = 1; cat <= NUM_CATEGORIES; cat++)
    {
        if (categoryTotal[cat] > 0)
        {
            printf("%d. %s\n", influenceNumber, getCategoryName(cat));
            influenceNumber++;
        }
    }

    printf("\nReflection:\n");
    printf("Did you have enough information before making your decisions?\n");
    printf("Did group opinion or popularity affect your choice?\n");
    printf("Did you feel pressured to decide quickly?\n");
    printf("Did the source of a claim change how much you believed it?\n");

    printf("\nPractical strategies:\n");
    printf("Pause before deciding, check the facts from more than one source,\n");
    printf("and ask whether a choice still makes sense without the pressure.\n");
    printDivider();

    pressEnterToContinue();
}

/* ---------- View previous results ---------- */

void displayPreviousResults(void)
{
    int i;

    printf("\n");
    printDivider();
    printf("          PREVIOUS RESULTS\n");
    printDivider();

    if (recordCount == 0)
    {
        printf("No saved results yet.\n");
    }
    else
    {
        printf("%-4s %-8s %-24s %-8s %-8s %s\n",
               "No.", "ID", "Category", "Initial", "Final", "Result");
        for (i = 0; i < recordCount; i++)
        {
            printf("%-4d %-8d %-24s %-8d %-8d %s\n",
                   i + 1,
                   records[i].scenarioId,
                   getCategoryName(records[i].category),
                   records[i].initialChoice,
                   records[i].finalChoice,
                   records[i].changed ? "Changed" : "Kept");
        }
    }

    printDivider();
    pressEnterToContinue();
}

/* ---------- Main program ---------- */

int main(void)
{
    int choice;
    int loaded;

    loaded = loadResults();
    printf("Loaded %d previous record(s).\n", loaded);

    while (1)
    {
        displayMainMenu();
        choice = readIntInRange("Enter your choice: ", 1, 9);

        switch (choice)
        {
            case 1: socialPressureSimulator();      break;
            case 2: fomoSimulator();                break;
            case 3: popularitySimulator();          break;
            case 4: authoritySimulator();           break;
            case 5: repeatedExposureSimulator();    break;
            case 6: generateAwarenessReport();      break;
            case 7:
                independentThinkingChecklist();
                pressEnterToContinue();
                break;
            case 8: displayPreviousResults();       break;
            case 9:
                printf("\nThank you for reflecting on your decisions. Goodbye!\n");
                return 0;
            default:
                printf("Unknown option.\n");
        }
    }
}