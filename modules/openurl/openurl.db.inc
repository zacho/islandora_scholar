<?php

/**
 * @file
 *
 * A collection of funcitons specific to this module, that make interacting with the database convenient.
 */
define('TABLE_OPENURL_SETTINGS', 'openurl_settings');
define('TABLE_OPENURL_DEFAULT_PARAMETERS', 'openurl_default_parameters');

/**
 * Gets all stored settings.
 *
 * @return stdClass
 *   A database query result resource, as returned from db_query().
 */
function openurl_db_get_settings() {
  return db_query('SELECT * FROM {%s}', TABLE_OPENURL_SETTINGS);
}

/**
 * Checks to see if the named setting exists.
 * 
 * @param string $name
 *   The settings name to look for.
 * 
 * @return boolean
 *   TRUE if successful FALSE otherwise.
 */
function openurl_db_has_setting($name) {
  $query = db_query('SELECT count(name) FROM {%s} WHERE name = "%s"', TABLE_OPENURL_SETTINGS, $name);
  return (db_result($query) > 0) ? TRUE : FALSE;
}

/**
 * Adds a new setting to the database.
 * 
 * @param string $name
 *   The unique name for this setting.
 * @param string $url
 *   The base url for the link resolver.
 * @param string $icon
 *   The name of the icon to be used for this link.
 * 
 * @return boolean
 *   TRUE if successful FALSE otherwise.
 */
function openurl_db_add_setting($name, $url, $icon = NULL) {
  if (!openurl_db_has_setting($name)) {
    $setting = new stdClass();
    $setting->name = $name;
    $setting->url = $url;
    $setting->icon = $icon;
    return drupal_write_record(TABLE_OPENURL_SETTINGS, $setting) !== FALSE;
  }
}