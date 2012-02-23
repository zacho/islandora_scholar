<?php

/**
 * @file
 *
 * Implements hooks and callbacks for this module settings page.
 */
module_load_include('inc', 'openurl', 'openurl.db');

/**
 * Gets the form defintion for the settings page.
 *
 * @param array $form_state
 *   Drupal form state.
 *
 * @return array
 *   Drupal form definition.
 */
function openurl_settings_form(array &$form_state) {
  $settings = array(
    '#header' => array('Name', 'Base URL', 'Icon', 'Default', '', ''),
    '#theme' => THEME_OPENURL_FORM_TABLE,
  );
  $results = openurl_db_get_settings();
  while ($row = db_fetch_object($results)) {
    unset($row->id); // Don't display the id.
    $column_values = array();
    foreach ($row as $key => $value) {
      $column_values[$key] = array(
        '#value' => $value
      );
    }
    /**
     * @todo Determine how to get the default one.
     */
    $default = variable_get('openurl_default', NULL) == $row->name;
    $is_default = array(
      '#prefix' => '<b>',
      '#value' => t('Is Default'),
      '#suffix' => '</b>'
    );
    $set_default = array(
      '#type' => 'submit',
      '#value' => t('Set Default'),
      '#setting_name' => $row->name,
      '#name' => 'set-default-' . $row->name,
      '#validate' => array(), // Prevent default valiation.
      '#submit' => array('openurl_set_default'),
    );
    $settings[] = array_merge($column_values, array(
      '#tree' => TRUE,
      'default' => $default ? $is_default : $set_default,
      'edit' => array(
        '#type' => 'submit',
        '#value' => 'Edit',
        '#setting_name' => $row->name,
        '#name' => 'edit-' . $row->name,
        '#validate' => array(), // Prevent default valiation.
        '#submit' => array('openurl_edit_setting_redirect'),
      ),
      'delete' => array(
        '#type' => 'submit',
        '#value' => 'Delete',
        '#setting_name' => $row->name,
        '#name' => 'delete-' . $row->name,
        '#validate' => array(), // Prevent default valiation.
        '#submit' => array('openurl_delete_setting'),
        ))
    );
  }
  $settings['add'] = array(
    'add_name' => array(
      '#type' => 'textfield',
      '#title' => 'Name',
      '#description' => t('Must be unique'),
      '#size' => 32,
      '#default_value' => isset($form_state['values']['name']) ? $form_state['values']['name'] : NULL,
    ),
    'add_url' => array(
      '#type' => 'textfield',
      '#title' => 'Base URL',
      '#description' => t('Link Resolver Address'),
      '#size' => 32,
      '#default_value' => isset($form_state['values']['name']) ? $form_state['values']['name'] : NULL,
    ),
    'file' => array(
      '#type' => 'file',
      '#title' => 'Icon',
      '#description' => t('Icon for the link.'),
      '#default_value' => isset($form_state['values']['name']) ? $form_state['values']['name'] : NULL,
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => 'Add',
      '#submit' => array('openurl_add_setting'),
      '#validate' => array('openurl_add_setting_validate'),
    ),
    0 => array(
      '#value' => '' // Filler so the table is formated correctly.
    ),
    1 => array(
      '#value' => '' // Filler so the table is formated correctly.
    ),
  );
  $form = array(
    'fieldset' => array(
      '#title' => t('Settings'),
      '#type' => 'fieldset',
      'settings' => $settings
    ),
    /**
     * Default action is to add a setting
     */
    '#attributes' => array('enctype' => 'multipart/form-data'),
    '#submit' => array('openurl_add_setting'),
    '#validate' => array('openurl_add_setting_validate'),
  );  
  return $form;
}

/**
 * 
 * 
 * @param array $form_state
 *   Drupal form state.
 * @param string $name
 *   The name of the settings to edit.
 *
 * @return array
 *   Drupal form definition.
 */
function openurl_default_parameters_form(array &$form_state, $name) {
  $form = array();
  return $form;
}

/**
 * Validates the input for adding a new setting.
 * 
 * @param array $form
 *   The Drupal form.
 * @param array $form_state 
 *   The Drupal form state.
 */
function openurl_add_setting_validate(array $form, array &$form_state) {
  $name = $form_state['values']['add_name'];
  if (empty($name)) {
    form_set_error('add_name', t('field is required.'));
  }
  if (openurl_db_has_setting($name)) {
    form_set_error('add_name', t('Settings for "%name" already exists.', array('%name' => $name)));
  }
  /**
   * @todo fix fileuploads.
   */
  $form_state['values']['file'] = file_save_upload('file', array(
    'file_validate_is_image' => array(),
  ), file_directory_path() . '/openurl', FILE_EXISTS_REPLACE);
}

/**
 * Adds a new setting for OpenURL.
 * 
 * @param array $form
 *   The Drupal form.
 * @param array $form_state 
 *   The Drupal form state.
 */
function openurl_add_setting(array $form, array &$form_state) {
  $values = $form_state['values'];
  $filename = $values['file']->filename;
  $added = openurl_db_add_setting($values['add_name'], $values['add_url'], $filename);
  $message = $added ? 'Successfully added settings for "%name"' : 'Failed to add settings for "%name"';
  $message_args = array('%name' => $values['add_name']);
  // check for file.
  drupal_set_message(t($message, $message_args));
}

/**
 * Redirects the user to the edit page for a given element.
 * 
 * @param array $form
 * @param array $form_state 
 */
function openurl_edit_setting_redirect(array $form, array &$form_state) {
  $name = $form_state['clicked_button']['#setting_name'];
  drupal_goto(str_replace('%', $name, MENU_OPENURL_DEFAULT_PARAMETERS));
}

/**
 * Sets the default OpenURL to use when none is specified.
 * 
 * @param array $form
 *   The Drupal form.
 * @param array $form_state 
 *   The Drupal form state.
 */
function openurl_set_default(array $form, array &$form_state) {
  $name = $form_state['clicked_button']['#setting_name'];
  $exists = openurl_db_has_setting($name);
  if ($exists) {
    variable_set('openurl_default', $name);
    $message = 'Successfully changed the default OpenURL to "%name"';
    $type = 'status';
  }
  else {
    $message = 'Failed to change the default OpenURL to "%name"';
    $type = 'error';
  }
  $message_args = array('%name' => $name);
  drupal_set_message(t($message, $message_args), $type);
}

/**
 * Theme's a form table for this module.
 * 
 * @param array $element
 *   A Drupal Form Element.
 * 
 * @return sting
 *   HTML that renders a table of settings for datastreams.
 */
function theme_openurl_form_table(array $element) {
  $rows = array();
  foreach (element_children($element) as $child) {
    $setting = $element[$child];
    $fields = array();
    foreach (element_children($setting) as $property) {
      $field = $setting[$property];
      $fields[] = drupal_render($field);
    }
    $rows[] = array(
      'data' => $fields,
      'class' => isset($setting['#attributes']['class']) ? $setting['#attributes']['class'] : NULL
    );
  }
  $attributes = isset($element['#id']) ? array('id' => $element['#id']) : NULL;
  return theme_table($element['#header'], $rows, $attributes);
}

/**
 * Renders both admin forms, used for editing existing OpenURL settings.
 * 
 * @param string $name
 *   The name of the OpenURL settings.
 * 
 * @return string 
 *   Returns the html for both admin forms.
 */
function theme_openurl_form_default_parameters($name) {
  if (!openurl_db_has_setting($name)) {
    drupal_not_found();
    return;
  }
  else {
    $output = drupal_get_form('openurl_settings_form');
    $output .= drupal_get_form('openurl_default_parameters_form', $name);
    return $output;
  }
}