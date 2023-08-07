import java.sql.*;
import java.util.Scanner;

public class IMT2020061_MainMenu {

   static final String JDBC_DRIVER = "com.mysql.cj.jdbc.Driver";
   static int availableBooks = 0;
   static final String DB_URL = "jdbc:mysql://localhost/Library?allowPublicKeyRetrieval=true&useSSL=false";

   static final String USER = "root";
   static final String PASS = "dilshu";

   public static void main(String[] args) {
      Connection conn = null;
      Statement stmt = null;

      try {
         Class.forName(JDBC_DRIVER);
         conn = DriverManager.getConnection(DB_URL, USER, PASS);

         stmt = conn.createStatement();

         Scanner scan = new Scanner(System.in);
         System.out.println("Welcome to my Library!!\n");
         System.out.println("Choose any one of the following choice:\n");
         System.out.print("Login as:\n\n");
         System.out.print("1. Student\n");
         System.out.print("2. Library_Owner\n");
         System.out.print("3. Exit\n\n");

         System.out.print("Your choice: ");
         int choice = scan.nextInt();
         if (choice == 1) {
            System.out.println("\nYou are logged in as a student now\n");
            stud_Login(stmt, scan);
         } else if (choice == 2) {
            System.out.println("\nYou are logged in as a Library Owner now\n");
            owner_Login(stmt, scan);
         } else {
            stmt.close();
            conn.close();
            System.exit(0);
         }

         stmt.close();
         conn.close();
      } catch (SQLException se) {
         se.printStackTrace();
      } catch (Exception e) {
         e.printStackTrace();
      } finally {
         try {
            if (stmt != null)
               stmt.close();
         } catch (SQLException se2) {
         }
         try {
            if (conn != null)
               conn.close();
         } catch (SQLException se) {
            se.printStackTrace();
         }
      }
      System.out.println("End of Code");
   }

   static void stud_Login(Statement stmt, Scanner scan) {
      System.out.println("Choose any of the following choices:\n");
      System.out.println("1. Get book by ISBN");
      System.out.println("2. Issue book");
      System.out.println("3. Exit");
      System.out.print("\nYour choice: ");
      int choice = scan.nextInt();
      while (choice != 3) {
         if (choice == 1) {
            getBook_byISBN(stmt, scan);
         } else if (choice == 2) {
            issue_Book(stmt, scan);
         } else {
            System.exit(0);
         }
         System.out.print("\nEnter your next choice: ");
         choice = scan.nextInt();
      }
   }

   static void owner_Login(Statement stmt, Scanner scan) {
      System.out.println("Choose any of the following choices:\n");
      System.out.println("1. Add book\n");
      System.out.println("2. Delete book\n");
      System.out.println("3. Add Student\n");
      System.out.println("4. Delete Student\n");
      System.out.println("5. Get all Students\n");
      System.out.println("6. Get all Books\n");
      System.out.println("7. Update cgpa of a student\n");
      System.out.println("8. Exit\n");

      System.out.print("Your choice: ");
      int choice = scan.nextInt();
      while (choice != 8) {
         if (choice == 1) {
            add_Book(stmt, scan);
         } else if (choice == 2) {
            delete_Book(stmt, scan);
         } else if (choice == 3) {
            add_Stud(stmt, scan);
         } else if (choice == 4) {
            delete_Stud(stmt, scan);
         } else if (choice == 5) {
            getAll_Stud(stmt, scan);
         } else if (choice == 6) {
            getAll_Books(stmt, scan);
         } else if (choice == 7) {
            updateCGPA(stmt, scan);
         } else {
            System.exit(0);
         }
         System.out.print("\nYour next choice: ");
         choice = scan.nextInt();
      }
      System.exit(0);
   }

   static void getBook_byISBN(Statement stmt, Scanner scan) {
      try {
         System.out.print("\nEnter ISBN of the book: ");
         int ISBN = scan.nextInt();
         String sql = String.format("SELECT * from BOOK where isbn = %d", ISBN);
         ResultSet rs = stmt.executeQuery(sql);

         int i = 0;
         while (rs.next()) {
            i = 1;
            String Title = rs.getString("title");
            String Auth = rs.getString("author");
            float Price = rs.getFloat("price");
            int Isbn = rs.getInt("isbn");

            System.out.print("Title: " + Title + ",");
            System.out.print(" Auth: " + Auth + ",");
            System.out.print(" Price: " + Price + ",");
            System.out.print(" Isbn: " + Isbn);
         }
         System.out.print("\n");
         if (i == 0)
            System.out.println("No book with given isbn found");
      } catch (Exception e) {

      }
   }

   static void issue_Book(Statement stmt, Scanner scan) {
      try {
         scan.nextLine();
         System.out.print("Enter your Roll Number: ");
         String rollNo = scan.nextLine();
         System.out.println("\nList of available books that you can issue:\n");
         available_Books(stmt, scan);
         if (availableBooks > 0) {
            System.out.print("Enter ISBN of the book that you want to issue: ");
            int Isbn = scan.nextInt();
            String sql1 = String.format("UPDATE STUDENT SET isbn = %d where roll_no = %s", Isbn, rollNo);
            String sql2 = String.format("UPDATE BOOK SET issued = true where isbn = %d", Isbn);
            int res1 = 0, res2 = 0;
            res1 = stmt.executeUpdate(sql1);
            if (res1 == 1)
               res2 = stmt.executeUpdate(sql2);
            if (res2 == 0) {
               System.out.println("Error while issuing the book");
            } else {
               System.out.println("Book has been issued");
            }
         }
      } catch (Exception e) {

      }
   }
   
   static void updateCGPA(Statement stmt, Scanner scan) {
      try {
         scan.nextLine();
         System.out.print("Enter Roll Number of the student: ");
         String rollNo = scan.nextLine();
         System.out.print("Enter new CGPA of the student: ");
         float cg = scan.nextFloat();
         String sql = String.format("UPDATE STUDENT SET cgpa = %f where roll_no = %s", cg, rollNo);
         int res = stmt.executeUpdate(sql);
         if(res == 1) System.out.println("\nCGPA updated");
         else System.out.println("\nCGPA couldn't be updated");
      } catch (Exception e) {

      }
   }

   static void add_Book(Statement stmt, Scanner scan) {
      try {
         scan.nextLine();
         System.out.print("Enter Title of the book: ");
         String title = scan.nextLine();
         System.out.print("Enter Author of the book: ");
         String auth = scan.nextLine();
         System.out.print("Enter price of the book: ");
         float price = scan.nextFloat();
         System.out.print("Enter isbn of the book: ");
         int isbn = scan.nextInt();

         String sql = String.format("INSERT INTO BOOK VALUES('%s', '%s', '%f', '%d', false);", title, auth, price,
               isbn);
         int res = stmt.executeUpdate(sql);
         if (res != 0)
            System.out.println("Book has been added");
         else
            System.out.println("Book couldn't be added");
      } catch (Exception e) {

      }
   }

   static void delete_Book(Statement stmt, Scanner scan) {
      try {
         System.out.print("Enter isbn of the book: ");
         int isbn_Book = scan.nextInt();

         String sql = String.format("DELETE FROM BOOK WHERE isbn = %d", isbn_Book);
         int res = stmt.executeUpdate(sql);

         if (res != 0)
            System.out.println("Book has been deleted");
         else
            System.out.println("Book couldn't be deleted");
      } catch (Exception e) {
         System.out.println("\nCannot delete book");
      }
   }

   static void add_Stud(Statement stmt, Scanner scan) {
      try {
         scan.nextLine();
         System.out.print("Enter Name of the student: ");
         String name = scan.nextLine();
         System.out.print("Enter Roll Number of the student: ");
         String rollNo = scan.nextLine();
         System.out.print("Enter cgpa of the student: ");
         float cg = scan.nextFloat();

         String sql = String.format("INSERT INTO STUDENT VALUES('%s', '%s', '%f', NULL);", name, rollNo, cg);
         int res = stmt.executeUpdate(sql);
         if (res != 0)
            System.out.println("Student has been added");
         else
            System.out.println("Student couldn't be added");
      } catch (Exception e) {

      }
   }

   static void delete_Stud(Statement stmt, Scanner scan) {
      try {
         scan.nextLine();
         System.out.print("\nEnter RollNo of the Student: ");
         String rollNo = scan.nextLine();

         String sql = String.format("DELETE FROM STUDENT WHERE roll_no = %s", rollNo);

         int res = stmt.executeUpdate(sql);

         if (res != 0)
            System.out.println("Student has been deleted");
         else
            System.out.println("Student couldn't be deleted");

      } catch (Exception e) {
         System.out.println(e);
      }
   }

   static void getAll_Stud(Statement stmt, Scanner scan) {
      try {
         String sql = "SELECT * from STUDENT";
         ResultSet rs = stmt.executeQuery(sql);
         int i = 0;
         while (rs.next()) {
            i = 1;
            String name = rs.getString("name");
            String rollNo = rs.getString("roll_no");
            float cg = rs.getFloat("cgpa");
            int Isbn = rs.getInt("isbn");

            System.out.print("Name: " + name + ",");
            System.out.print(" Roll Number: " + rollNo + ",");
            System.out.print(" Cgpa: " + cg + ",");
            System.out.print(" ISBN: " + Isbn);
            System.out.print("\n");
         }
         if (i == 0)
            System.out.println("No student found\n");
         rs.close();
      } catch (Exception e) {

      }
   }

   static void getAll_Books(Statement stmt, Scanner scan) {
      try {

         String sql = "SELECT * from BOOK";
         ResultSet rs = stmt.executeQuery(sql);

         int i = 0;
         while (rs.next()) {
            i = 1;
            String Title = rs.getString("title");
            String Auth = rs.getString("author");
            float Price = rs.getFloat("price");
            int Isbn = rs.getInt("isbn");

            System.out.print("Title: " + Title + ",");
            System.out.print(" Author: " + Auth + ",");
            System.out.print(" Price: " + Price + ",");
            System.out.print(" Isbn: " + Isbn);
            System.out.print("\n");
         }
         if (i == 0)
            System.out.println("No books found\n");
         rs.close();
      } catch (Exception e) {

      }
   }

   static void available_Books(Statement stmt, Scanner scan) {
      try {
         String sql = "SELECT * from BOOK where issued = false";
         ResultSet rs = stmt.executeQuery(sql);
         int i = 0;
         while (rs.next()) {
            i = 1;
            String Title = rs.getString("title");
            String Auth = rs.getString("author");
            float Price = rs.getFloat("price");
            int Isbn = rs.getInt("isbn");

            System.out.print("Title: " + Title + ",");
            System.out.print(" Auth: " + Auth + ",");
            System.out.print(" Price: " + Price + ",");
            System.out.println(" Isbn: " + Isbn);
         }
         System.out.print("\n");
         if (i == 0) {
            System.out.println("Sry, No available books currently!");
            availableBooks = 0;
         } else
            availableBooks = 1;
      } catch (Exception e) {

      }
   }

}