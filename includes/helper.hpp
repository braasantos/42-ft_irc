#pragma once

#include "utils.hpp"

/**
 ** Fucntion to check if the port was inside the range (6660-6669 , 7000)
 ** and check if it was only numbers.
 *
 * @param port String with the port to be checked.
 * @return true
 * @return false
 */
bool checkValidPort(string port);

/**
 ** Function that prints the error mensage and exit the code.
 *
 * @param error String with the error to be printed.
 */
void p_error(string error);

/**
 ** Function that check if it was a strong password.
 ** (-1 Upper Case Letter)
 ** (-1 Lower Case Letter)
 ** (-1 Number)
 ** (At least 8 characters)
 *
 * @param password String with the password to be checked.
 * @return true
 * @return false
 */
bool checkValidPassword(string password);

/**
 * @brief Handle the error connection.
 * 
 */
void handleErrorConnection();


/**
 * @brief Handle the SIGNAL stop connection.
 * 
 */
void signalHandler(int signum);
