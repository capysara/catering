<?php

/**
 * @file
 * Provides form fields, email address validation, custom submit message, and
 * database insert, and generates email when special instruction are included.
 */

/**
 * Fields for Catering Meal Choices form.
 */
function catering_meal_choices($form, &$form_state) {
  global $user;

  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Please provide your name, email, and meal preferences.'),
  );
  $form['contact_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Contact Information'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['contact_info']['first'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#required' => TRUE,
  );
  $form['contact_info']['last'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#required' => TRUE,
  );
  $form['contact_info']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#default_value' => (isset($user->mail)) ? $user->mail : '',
    '#required' => TRUE,
  );
  $form['meal_type'] = array(
    '#type' => 'radios',
    '#title' => t('Type of meal'),
    '#description' => t('Select your preferred meal type for this event.'),
    '#options' => array(
      0 => t('Meat'),
      1 => t('Fish'),
      2 => t('Vegetarian'),
   ),
   '#required' => TRUE,
  );
  $form['diet_restrict'] = array(
    '#type' => 'select',
    '#title' => t('Dietary restrictions'),
    '#description' => t('Specify any dietary restrictions.'),
    '#options' => array(
      '0' => t('None'),
      '1' => t('Gluten-free'),
      '2' => t('Dairy-free'),
   ),
   '#required' => TRUE,
  );
  $form['special_instruct'] = array(
    '#type' => 'textarea',
    '#title' => t('Special instructions or other dietary restrictions'),
    '#description' => t('Indicate if you have any other dietary needs.'),
    '#required' => FALSE,
  );

  // Alpha sort meal type options.
  natcasesort($form['meal_type']['#options']);

  // Alpha sort for diet restrictions, except keep None as first option.
  $diet_restrict = $form['diet_restrict']['#options'];
  natcasesort($diet_restrict);
  $form['diet_restrict']['#options'] = ['0' => $diet_restrict['0']] + $diet_restrict;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

/**
 * Find all users with role specified in variable 'catering_notify_role'
 * for email notifications.
 */
function _catering_users_by_role($rid) {
  $role = user_role_load($rid);
  $query = 'SELECT ur.uid
    FROM {users_roles} AS ur
    WHERE ur.rid = :rid';
  $result = db_query($query, array(':rid' => $role->rid));
  $uids = $result->fetchCol();

  return user_load_multiple($uids);
 }

/**
 * Validation handler for email address.
 */
function catering_meal_choices_validate($form, &$form_state) {
  if ($form_state['values']['email'] && !valid_email_address($form_state['values']['email'])) {
    form_set_error('email', t('Enter a valid email address.'));
  }
}

/**
 * Submit function for catering meal choices.
 */
function catering_meal_choices_submit($form, &$form_state) {
  drupal_set_message(t('Thank you! The meal order for @first @last has been submitted.',
    array(
      '@first' => $form_state['values']['first'],
      '@last' => $form_state['values']['last'],
    )
  ));

  // Insert the suggestions.
  $ifields = [];
  $ifields['first'] = $form_state['values']['first'];
  $ifields['last'] = $form_state['values']['last'];
  $ifields['email'] = $form_state['values']['email'];
  $ifields['meal_type'] = $form_state['values']['meal_type'];
  $ifields['diet_restrict'] = $form_state['values']['diet_restrict'];
  $ifields['special_instruct'] = $form_state['values']['special_instruct'];

  $query = db_insert('catering_meal_choices')->fields($ifields);
  $query->execute();

  // If there are special instructions and a role has been selected to receive
  // email notifications, send an email.
  if (variable_get('catering_notify_role') && $form_state['values']['special_instruct']) {
    catering_mail_send($form_state['values']);
  }
}

/**
 * Implements hook_mail().
 */
function catering_mail($key, &$message, $params) {

  $options = array(
    'langcode' => $message['language']->language,
  );

  switch ($key) {
    // Send a message from meal choices form, including special instructions.
    case 'catering_meal_choices':
      $message['subject'] = t('@site-name | New special instructions | @date',
        array(
          '@site-name' => variable_get('site_name', 'Drupal'),
          '@date' => date('m/d/Y')), $options);
      $message['body'][] = t(
        'A meal order has been placed with special instructions:', $options);
      $message['body'][] = check_plain($params['special_instruct']);
      break;
  }
}

/**
 * Sends an e-mail to all users of specified role.
 */
function catering_mail_send($form_values) {

  $module = 'catering';
  $key = 'catering_meal_choices';

  // The To field will be populated with email addresses for users with the
  // designated role. Defaults to administrator.
  $notify_roles = _catering_users_by_role(variable_get('catering_notify_role'));

  $to = array();
  foreach ($notify_roles as $notify_role => $value) {
    $to[] = $value->mail;
  }

  // To field must be a comma-separated string, not an array.
  $to = implode(",", $to);

  $from = variable_get('site_mail', 'admin@example.com');

  $params = $form_values;

  $language = language_default();

  $send = TRUE;

  $result = drupal_mail($module, $key, $to, $language, $params, $from, $send);

}
