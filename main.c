#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DISCOUNT 1

// system("cls"); (For Windows)
// system("clear"); (Other MacOS/Linux)

typedef struct {
  int year;
  int month;
  int day;
} Date;

typedef struct {
  char name[50];
  float price;
  Date date;
} Movie;

void menu();
void manageWallet();
void changeBalance();
void checkAvailableTickets();
void bookTicket(int x);
void manageBookings();
void initialiseMovies();
void initialiseBooking();
void returnToMenu();
void removeBooking(int x);

int selection;
char coupon[] = "NSU100";

Movie movies[4];

int main() {
  initialiseMovies();
  menu();
  return 0;
}

void menu() {
  printf("Welcome to the NSU Cinema!");
  printf("\n\nPlease select the action you'd like to perform:");
  printf("\n");
  printf("\n1. Manage your wallet");
  printf("\n2. Check avaliable tickets and book a ticket");
  printf("\n3. Manage a booking");
  printf("\n4. Setup/Reset booking system");
  printf("\n");

  printf("\nPlease enter your selection: ");
  scanf("%d", &selection);

  while (selection < 1 || selection > 4) {
    printf("Invalid selection! Please enter again: ");
    scanf("%d", &selection);
  }

  switch (selection) {
  case 1:
    system("clear");
    manageWallet();
    break;
  case 2:
    system("clear");
    checkAvailableTickets();
    break;
  case 3:
    system("clear");
    manageBookings();
    break;
  case 4:
    initialiseBooking();
    break;
  default:
    printf("\n\nSomething went wrong.");
    break;
  }
}

void manageWallet() {
  FILE *readfile = fopen("balance.txt", "r");
  float current_bal = 0;
  fscanf(readfile, "%f", &current_bal);
  fclose(readfile);

  printf("Your current balance is $%.2f.", current_bal);
  printf("\n\nPlease select the action you'd like to perform:");
  printf("\n\n1. Add or remove balance (use -ve number to deduct)");
  printf("\n0. Return back to main menu.");
  printf("\n\n");
  printf("Enter your selection: ");
  scanf("%d", &selection);

  while (selection != 1 && selection != 0) {
    printf("Invalid selection! Please enter again: ");
    scanf("%d", &selection);
  }

  switch (selection) {
  case 1:
    changeBalance();
    break;
  case 0:
    system("clear");
    menu();
    break;
  default:
    printf("\n\nSomething went wrong.");
    break;
  }
}

void changeBalance() {
  float change_bal;
  printf("\nPlease enter the amount you'd like to add or deduct: ");
  scanf("%f", &change_bal);

  FILE *readfile = fopen("balance.txt", "r");
  float current_bal = 0;
  fscanf(readfile, "%f", &current_bal);
  float new_bal = current_bal + change_bal;
  fclose(readfile);

  FILE *writefile = fopen("balance.txt", "w");
  fprintf(writefile, "%.2f", new_bal);
  fclose(writefile);

  if (change_bal < 0) {
    printf("\nSuccessfully removed $%.2f from the balance.\nNew balance: $%.2f",
           change_bal, new_bal);
  } else if (change_bal == 0) {
    printf("\nBalance unchanged.");
  } else if (change_bal > 0) {
    printf("\nSuccessfully added $%.2f to the balance.\nNew balance: $%.2f",
           change_bal, new_bal);
  } else {
    printf("\n\nSomething went critically wrong.");
  }

  returnToMenu();
}

void checkAvailableTickets() {
  printf("Please select the movie you'd like to book a ticket for.");
  printf("\n");

  for (int i = 0; i < 4; i++) {
    printf("\n%d. %s - %d/%d/%d - $%.2f", (i + 1), movies[i].name,
           movies[i].date.day, movies[i].date.month, movies[i].date.year,
           movies[i].price);
  }

  printf("\n\n0. Return back to main menu");

  printf("\n\nEnter your selection: ");
  scanf("%d", &selection);

  while (selection < 0 || selection > 4) {
    printf("Invalid selection! Please enter again: ");
    scanf("%d", &selection);
  }

  bookTicket(selection);
};

void bookTicket(int x) {
  if (x == 0) {
    system("clear");
    menu();
    return;
  }

  float balance;
  int bookings[4][2];

  FILE *ftpr = fopen("bookings.txt", "r");

  for (int i = 0; i < 4; i++) {
    fscanf(ftpr, "%d %d", &bookings[i][0], &bookings[i][1]);
  }

  fclose(ftpr);

  if (bookings[x - 1][1] == 1) {
    system("clear");
    printf("You've already booked a ticket for %s!\n\n", movies[x - 1].name);
    checkAvailableTickets();
    return;
  }

  ftpr = fopen("balance.txt", "r");
  fscanf(ftpr, "%f", &balance);
  fclose(ftpr);

  char couponInput[50];
  float priceToPay = movies[x - 1].price;

  printf(
      "Do you have a coupon to use?\n\n1. Yes\n2. No\n\nEnter your selection:");
  scanf("%d", &selection);

  if (selection == 1) {
    printf("Enter the coupon code: ");
    scanf("%s", couponInput);

    if (strcmp(couponInput, "NSU100") == 0) {
      printf("Coupon applied! You get $%.2f off.\n", (float)DISCOUNT);
      priceToPay -= DISCOUNT;
    } else {
      printf("Invalid coupon code. Proceeding without discounts.");
    }
  }

  ftpr = fopen("balance.txt", "w");
  float new_bal = balance - priceToPay;

  if (balance < priceToPay) {
    system("clear");
    printf("You do not have enough balance to purchase a ticket for %s!\nYour "
           "current balance is $%.2f.\nPlease return to main menu and add some "
           "money.\n\n",
           movies[x - 1].name, balance);
    checkAvailableTickets();
    return;
  }

  fprintf(ftpr, "%.2f", new_bal);
  fclose(ftpr);

  printf("\n\n=====================================\n");
  printf("         NSU Cinema\n");
  printf("   Bashundhara, Dhaka-1229, Bangladesh\n");
  printf("       Tel: +880-2-55668200\n");
  printf("=====================================\n");
  printf("Item            Qty     Price   Total\n");
  printf("-------------------------------------\n");

  printf("%s          1       $%.2f    $%.2f\n", movies[x - 1].name, movies[x - 1].price, movies[x - 1].price);

  printf("-------------------------------------\n");
  printf("Subtotal:                       $%.2f\n", movies[x - 1].price);

  if (priceToPay < movies[x - 1].price) {
    printf("Discount ($%d):                  -$%.2f\n", DISCOUNT, (float)DISCOUNT);
  } else {
    printf("Discount ($0):                  -$0.00\n");
  }
  
  printf("=====================================\n");
  printf("Grand Total:                   $%.2f\n", priceToPay);
  printf("=====================================\n");
  printf("      Thank you for booking!\n");
  printf("=====================================\n\n");

  bookings[x - 1][1] = 1;

  ftpr = fopen("bookings.txt", "w");
  for (int i = 0; i < 4; i++) {
    fprintf(ftpr, "%d %d\n", bookings[i][0], bookings[i][1]);
  }

  fclose(ftpr);

  checkAvailableTickets();
};

void manageBookings() {
  printf("These are your currently booked tickets:\n");

  int bookings[4][2];
  FILE *ftpr = fopen("bookings.txt", "r");

  for (int i = 0; i < 4; i++) {
    fscanf(ftpr, "%d %d", &bookings[i][0], &bookings[i][1]);
  }

  fclose(ftpr);

  int noBookings = 0;

  for (int i = 0; i < 4; i++) {
    if (bookings[i][1] == 1) {
      printf("%d. %s - %d/%d/%d\n", bookings[i][0],
             movies[bookings[i][0] - 1].name,
             movies[bookings[i][0] - 1].date.day,
             movies[bookings[i][0] - 1].date.month,
             movies[bookings[i][0] - 1].date.year);
      noBookings = 1;
    }
  }

  if (noBookings == 0) {
    printf("You currently do not have any bookings.\nPlease return back to "
           "main menu and book a ticket.");
    returnToMenu();
    return;
  } else {
    printf("\nUpon cancellation, you will receive an 80 percent "
           "refund.\nPlease select the booking you'd like to remove, or enter "
           "0 to return back to the main menu: ");

    int valid = 0;
    while (!valid) {
      printf("Enter your selection: ");
      scanf("%d", &selection);

      if (selection == 0) {
        system("clear");
        menu();
        return;
      }

      if (selection >= 1 && selection <= 4 && bookings[selection - 1][1] == 1) {
        valid = 1;
        removeBooking(selection);
      } else {
        printf("Invalid selection! Please select a booking you have already "
               "made.\n");
      }
    }
  }
};

void initialiseMovies() {
  strcpy(movies[0].name, "The Lorax");
  movies[0].price = (float)20.00;
  movies[0].date.day = 15;
  movies[0].date.month = 12;
  movies[0].date.year = 2025;

  strcpy(movies[1].name, "The Lego Batman Movie");
  movies[1].price = (float)25.00;
  movies[1].date.day = 16;
  movies[1].date.month = 12;
  movies[1].date.year = 2025;

  strcpy(movies[2].name, "Titanic");
  movies[2].price = (float)5.00;
  movies[2].date.day = 17;
  movies[2].date.month = 12;
  movies[2].date.year = 2025;

  strcpy(movies[3].name, "The Lion King");
  movies[3].price = (float)15.00;
  movies[3].date.day = 18;
  movies[3].date.month = 12;
  movies[3].date.year = 2025;
};

void initialiseBooking() {
  FILE *ftprx = fopen("bookings.txt", "w");
  for (int i = 1; i < 5; i++) {
    fprintf(ftprx, "%d 0\n", i);
  }

  fclose(ftprx);

  printf("Initialised!\n");
  system("clear");
  menu();
};

void returnToMenu() {
  printf("\n\nPlease enter 0 to return back to main menu: ");
  scanf("%d", &selection);

  while (selection != 0) {
    printf("Invalid selection! Please enter again: ");
    scanf("%d", &selection);
  }

  system("clear");
  menu();
}

void removeBooking(int x) {
  int bookings[4][2];
  float balance;

  FILE *ftpr = fopen("bookings.txt", "r");
  for (int i = 0; i < 4; i++) {
    fscanf(ftpr, "%d %d", &bookings[i][0], &bookings[i][1]);
  }

  fclose(ftpr);

  bookings[x - 1][1] = 0;

  ftpr = fopen("bookings.txt", "w");
  for (int i = 0; i < 4; i++) {
    fprintf(ftpr, "%d %d\n", bookings[i][0], bookings[i][1]);
  }

  fclose(ftpr);

  ftpr = fopen("balance.txt", "r");
  fscanf(ftpr, "%f", &balance);
  fclose(ftpr);

  float refund = movies[x - 1].price * 0.8f;
  balance += refund;

  ftpr = fopen("balance.txt", "w");
  fprintf(ftpr, "%.2f", balance);

  fclose(ftpr);

  printf("\nBooking for %s has been cancelled.\nYou have been refunded $%.2f "
         "(80 percent of ticket price).\nYour new balance is $%.2f.",
         movies[x - 1].name, refund, balance);

  returnToMenu();
}


  
