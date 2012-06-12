<?php

// $Id$

/**
 * @file
 *
 * Implements hooks and callbacks for the admin section.
 */

/**
 * Build the Admin settings form.
 * 
 * @param array $form_state 
 * 
 * @return array
 */
function scholar_admin_settings(array &$form_state) {
  module_load_include('inc', 'scholar', 'citation/csl/CSL');
  $form = array();
  $form['scholar_solr'] = array(
    '#type' => 'fieldset',
    '#title' => t('Solr Fields'),
  );
/*
  $form['scholar_solr']['islandora_scholar_solr_title_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Title Search Field'),
    '#description' => t('The SOLR search field used for title.'),
    '#default_value' => variable_get(VAR_SCHOLAR_SEARCH_FIELD_TITLE, 'title'),
    '#required' => FALSE,
  );
*/
  $form['scholar_solr']['islandora_scholar_solr_author_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Author Solr Field'),
    '#description' => t('The SOLR search field used for author lookups.'),
    '#default_value' => variable_get('islandora_scholar_solr_author_field', 'mods_name_author_ms'),
    '#required' => FALSE,
  );
  $form['scholar_solr']['islandora_scholar_relsext_hasmodel_field'] = array(
    '#type' => 'textfield',
    '#title' => t('RELS-EXT Solr Field'),
    '#description' => t('The SOLR search field that contains RELS-EXT relationships from Fedora.'),
    '#default_value' => variable_get('islandora_scholar_relsext_hasmodel_field', 'rels_hasModel_uri_ms'),
    '#required' => FALSE,
  );
/*
  $form[VAR_SCHOLAR_SEARCH_FIELD_TEXT] = array(
    '#type' => 'textfield',
    '#title' => t('Text Search Field'),
    '#description' => t('The SOLR search field used for text.'),
    '#default_value' => variable_get(VAR_SCHOLAR_SEARCH_FIELD_TEXT, 'text'),
    '#required' => TRUE,
  );
  $form[VAR_SCHOLAR_SEARCH_FIELD_USER] = array(
    '#type' => 'textfield',
    '#title' => t('User Search Field'),
    '#description' => t('The SOLR search field used for user.'),
    '#default_value' => variable_get(VAR_SCHOLAR_SEARCH_FIELD_USER, 'user'),
    '#required' => TRUE,
  );
*/
  $form['scholar_citation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Citeproc'),
  );
  $form['scholar_citation']['islandora_scholar_theme_citation_from_solr_mods'] = array(
    '#type' => 'checkbox',
    '#title' => t('Build citations from Solr MODS?'),
    '#description' => t('Check this box only if you store a complete MODS record in Solr for quicker access. If checked, please specify the field below.'),
    '#default_value' => variable_get('islandora_scholar_theme_citation_from_solr_mods', FALSE),
  );
  $form['scholar_citation']['islandora_scholar_mods_xml_solr_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr MODS Field'),
    '#description' => t('If you\'ve checked above, indicate the Solr field which contains full unadulterated MODS'),
    '#default_value' => variable_get('islandora_scholar_mods_xml_solr_field', NULL),
  );

  $form['romeo'] = array(
    '#type' => 'fieldset',
    '#title' => t('RoMEO settings'),
  );
  $form['romeo']['scholar_romeo_url'] = array(
    '#type' => 'textfield',
    '#title' => 'RoMEO API URL',
    '#default_value' => variable_get('scholar_romeo_url', ''),
    '#size' => 50,
  );
  $form['romeo']['scholar_romeo_key'] = array(
    '#type' => 'textfield',
    '#title' => 'RoMEO API Key',
    '#default_value' => variable_get('scholar_romeo_key', ''),
    '#size' => 20,
  );
  $form['romeo']['scholar_romeo_cache_time'] = array(
    '#type' => 'textfield',
    '#title' => 'RoMEO Content Cache Time',
    '#description' => t('Time, in days, to store cached RoMEO data'),
    '#default_value' => variable_get('scholar_romeo_cache_time', '2'),
    '#size' => 3,
  );
  
  return system_settings_form($form);
}