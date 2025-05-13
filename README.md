 
# 📨 Chat System Project (C++ & MySQL)

A **console-based messaging system** written in **modern C++**, designed to simulate a secure and realistic chat environment.
This project demonstrates robust programming practices with encryption, database management, user interaction, and unit testing.
Built as a learning tool and technical showcase.

<br><br>
## ✨ Key Features

* 👤 **User Management**

  * Account registration and secure login
  * Encrypted password storage
  * Friend list: add or remove users
    
* 💬 **Messaging System**

  * Send and receive messages
  * Encrypted message content for user privacy
  * Delete sent messages
  * Search messages by keyword
 
* 🛡️ **Logging System**
  
  * Inspired by a lightweight implementation from my previous project [user-auth-system](https://github.com/Marko-Programmer/user-auth-system)

* 🧾 **Database Integration**

  * Full CRUD operations via MySQL Connector/C++
  * `config.txt` used for flexible DB configuration
    
* 🛠️ **Console Interface**

  * Menu-driven interaction
  * Strong input validation and error feedback
    
* 🧪 **Unit Testing**

  * Implemented with Catch2 framework
  * Tests for core modules ensure reliable behavior
    
* 🔐 **Security & Privacy**

  * User passwords are hashed before storage
  * Message content is encrypted prior to insertion into the database

<br><br>
### 💡 Technical Highlights

* **Language**: C++17
* **Libraries**:

  * Catch2 — unit testing
  * MySQL Connector/C++
  * STL
    
* **Security**:

  * Password hashing 
  * Message encryption (content-level) 

<br><br>
## 🗃️ Database Setup (MySQL)
To set up the database:

1. Create the database manually:

<pre>CREATE DATABASE chatsystem;</pre>

<pre>USE chatsystem</pre>

2. Then run the provided schema.sql script to create the users table with the required fields.


<br><br>
## 🛠️ Configuration

1. Download and integrate MySQL Connector/C++ with your project.

2. 📄 Setup config.txt
Rename `config_template.txt` to `config.txt`, then fill in your MySQL credentials.

3. Install Catch2 (Catch2 is used for testing. It can be included as a single header or via CMake/vcpkg).
 

<br><br>
## 🧪 Unit Testing
Unit tests are written using Catch2. 
<br>
To compile and run the tests, use the following command:
<br><br>

1. <pre>g++ tests/test_utils.cpp src/utils.cpp  -o tests -std=c++17</pre>

2. <pre>./tests</pre>
<br><br>

### ✅ Why This Project Stands Out

* ** Secure by Design**: Passwords are hashed, and messages are encrypted before being stored in the database
* **Full Chat Experience**: Registration, friend management, secure messaging, and full message control  
* **Modular & Clean Code**: Well-separated logic and manageable files  
* **Testing-Oriented**: Ready-to-run test suite ensures confidence   
<br><br>

### 🔮 Potential Extensions (Not currently planned)

These are theoretical additions that could extend the system further:
- Multithreading for concurrent users
- Client-server architecture using TCP/IP
- GUI version using Qt or web technologies
 
<br><br>
## 📝 License
See license.txt for license details.

<br><br>
### 🙋‍♂️ Author
**Marko Zenchuk**
