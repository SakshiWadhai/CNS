"use strict";

let users = {};
let loginAttempts = {};
const MAX_ATTEMPTS = 3;
const LOCK_TIME = 30000; // 30 seconds
const PEPPER = "S3cr3tPepperValue"; // demo pepper

// Load users from localStorage on page start
function loadUsers() {
  const stored = localStorage.getItem("secureUsers");
  if (stored) users = JSON.parse(stored);
}
loadUsers();

// Save users to localStorage
function saveUsers() {
  localStorage.setItem("secureUsers", JSON.stringify(users));
}

// Generate random salt
function generateSalt(length = 16) {
  const charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  let salt = "";
  for (let i = 0; i < length; i++) {
    salt += charset.charAt(Math.floor(Math.random() * charset.length));
  }
  return salt;
}

// Hash password using SHA-256 (works on HTTPS or http://localhost)
async function hashPassword(password, salt) {
  const encoder = new TextEncoder();
  const data = encoder.encode(password + salt + PEPPER);
  const hashBuffer = await crypto.subtle.digest("SHA-256", data);
  const hashArray = Array.from(new Uint8Array(hashBuffer));
  return hashArray.map(b => b.toString(16).padStart(2, "0")).join("");
}

// Password strength checker (fixed lookaheads)
function isStrongPassword(password) {
  const strongRegex =
    /^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[@$!#%?&])[A-Za-z\d@$!#%?&]{8,}$/;
  return strongRegex.test(password);
}

// Toggle show/hide password
function togglePassword(id) {
  const input = document.getElementById(id);
  input.type = input.type === "password" ? "text" : "password";
}

// Sign-up logic
async function signUp() {
  const user = document.getElementById("signupUser").value.trim();
  const pass = document.getElementById("signupPass").value.trim();
  const msg = document.getElementById("signupMsg");

  if (!user || !pass) {
    msg.textContent = "⚠ Enter username and password.";
    msg.style.color = "red";
    return;
  }

  if (!isStrongPassword(pass)) {
    msg.textContent =
      "Weak password! Use A-Z, a-z, 0-9, symbol, and 8+ chars.";
    msg.style.color = "red";
    return;
  }

  if (users[user]) {
    msg.textContent = "User already exists!";
    msg.style.color = "red";
    return;
  }

  const salt = generateSalt();
  const hash = await hashPassword(pass, salt);
  users[user] = { hash, salt };
  saveUsers();

  msg.textContent = "Signup successful! You can login now.";
  msg.style.color = "green";
  console.log("Stored Users:", users);
}

// Login logic
async function login() {
  const user = document.getElementById("loginUser").value.trim();
  const pass = document.getElementById("loginPass").value.trim();
  const msg = document.getElementById("loginMsg");

  if (!users[user]) {
    msg.textContent = "User not found.";
    msg.style.color = "red";
    return;
  }

  if (!loginAttempts[user]) loginAttempts[user] = { count: 0, locked: false };

  if (loginAttempts[user].locked) {
    msg.textContent = "Account is temporarily locked. Try again later.";
    msg.style.color = "red";
    return;
  }

  const { hash, salt } = users[user];
  const enteredHash = await hashPassword(pass, salt);

  if (enteredHash === hash) {
    msg.textContent = "Login successful!";
    msg.style.color = "green";
    loginAttempts[user] = { count: 0, locked: false };
  } else {
    loginAttempts[user].count++;
    if (loginAttempts[user].count >= MAX_ATTEMPTS) {
      msg.textContent =
        " Account locked for 30 seconds due to multiple failed attempts.";
      msg.style.color = "red";
      loginAttempts[user].locked = true;

      setTimeout(() => {
        loginAttempts[user] = { count: 0, locked: false };
        msg.textContent = " You can try logging in again now.";
        msg.style.color = "green";
      }, LOCK_TIME);
    } else {
      // FIX: use backticks for template literal
      msg.textContent = `Wrong password! Attempts: ${loginAttempts[user].count}/${MAX_ATTEMPTS}`;
      msg.style.color = "red";
    }
  }
}
