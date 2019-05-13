INTRODUCTION
------------

The Catering module provides a custom form to collect meal choice data
prior to events, including first and last name, email address, meal type,
dietary restrictions, and any other special instructions. Email address is
validated. If the user is logged in, their email is used to prepopulate the
field. Meal type and dietary restrictions are lists that are sorted by alpha.
All fields are required, except for 'special instructions' text field. Data is
saved in a custom database table that will be queried to find the total number
of meals by type. Two tabs are provided in the main menu. The Meal Choices
tab displays for all users; the Meal Type Totals tab only displays for site
administrators.


REQUIREMENTS
------------

 * No special requirements.


INSTALLATION
------------

 * Install as you would normally install a contributed Drupal module. Visit:
   https://www.drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------

 * Configure roles that will receive email notifications when a user submits
   a meal choice that contains special instructions in
   Administration » Catering » Special instructions notification:

   - The role will default to 'administrator' (provided out of the box).

   - If your site does not have an 'administrator' role, you must choose another
   role to receive notifications, or select No notifications.

When enabled, the module will display two tabs in the main menu for the meal
choices form and the admin page of the meal types totals. To remove the form
and admin page, disable the module and clear caches.


MAINTAINERS
-----------

Current maintainer:
 * Sara Cartee-Kennedy (capysara) - https://www.drupal.org/user/3294343
