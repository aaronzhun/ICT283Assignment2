#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "WeatherStore.h"

/**
 * @file Menu.h
 * @author Lau Ze Zhun Aaron
 * @brief Command-line interaction and the required report formats
 */
/**
 * @brief Runs the five-option menu until the user chooses exit
 * @param data Loaded weather records used for the reports
 * @note Does not modify the supplied records
 * @note Option four is a progress stub and does not write a file
 * @note End of console input also ends the menu
 */
void RunMenu(const WeatherStore & data); // Pass the map by const reference rather than copying its trees

#endif
