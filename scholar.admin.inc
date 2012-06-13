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
  $solr_url = variable_get('islandora_solr_search_block_url', 'localhost:8080/solr');
  $http_pos = strpos($solr_url, 'http://');
  if ($http_pos === FALSE) {
    $solr_url = 'http://' . $solr_url;
  }
  $luke_url = $solr_url . '/admin/luke';
  $xml_doc = new DOMDocument();
  $xml_doc->load($luke_url, LIBXML_NOCDATA);
  $xpath = new DOMXPath($xml_doc);
  $query_all = '//lst[@name="fields"]/lst';
  $entries = $xpath->query($query_all);
  $solr_fields_all = array();
  foreach ($entries as $entry) {
    $name = $entry->getAttribute('name');
    $solr_fields_all[$name] = $name;
  }
  asort($solr_fields_all);
  $query_single_valued = '//lst[@name="fields"]/lst/str[@name="schema" and not(contains(.,"M"))]/..';
  $entries = $xpath->query($query_single_valued);
  $solr_fields_single_valued = array();
  foreach ($entries as $entry) {
    $name = $entry->getAttribute('name');
    $solr_fields_single_valued[$name] = $name;
  }
  asort($solr_fields_single_valued);
  
    
  $form = array();
  $form['scholar_solr'] = array(
    '#type' => 'fieldset',
    '#title' => t('Solr Fields'),
  );
  $form['scholar_solr']['islandora_scholar_solr_author_field'] = array(
    '#type' => 'select',
    '#title' => t('Stored Author Field'),
    '#description' => t('The SOLR search field used for author lookups.'),
    '#options' => $solr_fields_all,
    '#default_value' => variable_get('islandora_scholar_solr_author_field', 'mods_name_author_ms'),
    '#required' => FALSE,
  );
  $form['scholar_solr']['islandora_scholar_solr_title_field'] = array(
    '#type' => 'select',
    '#title' => t('Stored Title Field'),
    '#description' => t('The SOLR search field used for title lookups and sorting. This cannot be a multi-valued field!'),
    '#options' => $solr_fields_single_valued,
    '#default_value' => variable_get('islandora_scholar_solr_title_field', 'mods_title_mlt'),
    '#required' => FALSE,
  );
  $form['scholar_solr']['islandora_scholar_relsext_hasmodel_field'] = array(
    '#type' => 'select',
    '#title' => t('RELS-EXT Solr Field'),
    '#description' => t('The SOLR search field that contains RELS-EXT relationships from Fedora.'),
    '#options' => $solr_fields_all,
    '#default_value' => variable_get('islandora_scholar_relsext_hasmodel_field', 'rels_hasModel_uri_ms'),
    '#required' => FALSE,
  );

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

  // UPEI's LDAP configuration -- probably unneeded.
  /*
  $form['ldap'] = array(
    '#type' => 'fieldset',
    '#title' => t('LDAP settings'),
  );
  $form['ldap']['scholar_ldap_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Host'),
    '#default_value' => variable_get('scholar_ldap_host', ''),
  );
  $form['ldap']['scholar_ldap_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Port'),
    '#default_value' => variable_get('scholar_ldap_port', '389'),
  );
  $form['ldap']['scholar_ldap_basedn'] = array(
    '#type' => 'textfield',
    '#title' => t('Base DN'),
    '#default_value' => variable_get('scholar_ldap_basedn', ''),
  );
  */

  // UPEI's JODConverter code -- Leverages OpenOffice to convert documents to PDF (and PDF/A)
  /*
  $form['jodconverter'] = array(
    '#type' => 'fieldset',
    '#title' => t('JODConverter settings'),
  );
  $form['jodconverter']['scholar_jod_path'] = array(
    '#type' => 'textfield',
    '#title' => t('JODConverter path'),
    '#default_value' => variable_get('scholar_jod_path', 'localhost:8080/RuleEngineServlet/RuleEngine'),
  );
  */

  // UPEI's Scholar Cache functionality -- not yet sure where this is used.
  /*
  $form['scholar_cache'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cache settings'),
  );
  $form['scholar_cache']['scholar_researcher_citations_cache_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Researcher citations cache time in days'),
    '#default_value' => variable_get('scholar_researcher_citations_cache_time', '2'),
  );
  */

  return system_settings_form($form);
}
/*
function scholar_admin_settings_validate($form, $form_state) {
  if (!isset($form_state['values']['scholar_romeo_url'])) {
    form_set_error('url', t('URL cannot be empty!'));
  }
  if (!isset($form_state['values']['scholar_romeo_key'])) {
    form_set_error('key', t('Key cannot be empty!'));
  }
}
*/
