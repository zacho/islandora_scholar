<?php

/**
 * @file
 */

/**
 * Cron function to remove the policy datastream from an object if the 
 * datastream has the label 'POLICY' and the mods.embargo_date
 * index field has expired in the last week. If the cron job is set up
 * properly then we sould only ever need to check the last week. Checking
 * further back would increase the number of objects that would need
 * to be checked.
 * Added extra check for a supplemental information embargo.
 */
function scholar_cron_embargo_check() {
  module_load_include('inc', 'fedora_repository', 'api/fedora_item');

  $scholar_path = drupal_get_path('module', 'scholar');

  $solr_url = variable_get('islandora_solr_search_block_url', 'localhost:8080/solr');
  $http_pos = strpos($solr_url, 'http://');
  if ($http_pos === FALSE) {
    $solr_url = 'http://' . $solr_url;
  }
  
  $embargo_field = variable_get('islandora_scholar_embargo_date_field', '');
  $supp_embargo_field = variable_get('islandora_scholar_supp_embargo_date_field', '');

  if (!empty ($embargo_field)) {
    //$document_solr_query = $solr_url . '/select/?q='.$embargo_field.':[NOW/DAY-7DAY%20TO%20NOW/DAY]%20AND%20rels.hasModel:thesisCModel&fl=PID&rows=10000';
    $document_solr_query = $solr_url . '/select/?q='.$embargo_field.':[NOW/DAY-7DAY%20TO%20NOW/DAY]&fl=PID&rows=10000';
    $document_solr_result = @file_get_contents($document_solr_query);

    if ($document_solr_result != NULL) {
      $xml = new SimpleXMLElement($document_solr_result);
      $array = $xml->xpath('//doc');
      $array = array_filter($array);
      $pids = array();
      foreach ($array as $arr) {
        $pid = (string) $arr[0]->str;
        drupal_set_message(t('PID: @pid', array('@pid' => $pid)));
        $pids[] = $pid;
        $item = new Fedora_Item($pid);
        $ds_array = $item->get_datastreams_list_as_array();
        foreach ($ds_array as $key => $ds) {
          if ($key == 'POLICY') {
            $policy_details = $item->get_datastream_info('POLICY');
            drupal_set_message(t('Policy label: @label', array('@label' => $policy_details->datastream->label)));
            switch ($policy_details->datastream->label) {
              case 'POLICY':
              case 'Embargo policy - Both':
              $result_remove = $item->purge_datastream('POLICY');
              $result_add = $item->add_datastream_from_file($scholar_path . '/xacml/supplemental-embargo.xml', 'POLICY', 'Embargo policy - Supp');
              break;
              case 'Embargo policy - Doc';
              $result_remove = $item->purge_datastream('POLICY');
              break;
              case 'Embargo policy - Supp':
              drupal_set_message(t('Document embargo for object \'@pid\' already removed!', array('@pid' => $pid)), 'warning');
              break;
              default:
              drupal_set_message(t('Unknown policy on object \'@pid\', unable to remove!', array('@pid' => $pid)), 'warning');
            }
          }
        }
      }
    }
  }

  if (!empty ($supp_embargo_field)) {
    //$supp_solr_query = $solr_url . '/select/?q='.$supp_embargo_field.':[NOW/DAY-7DAY%20TO%20NOW/DAY]%20AND%20rels.hasModel:thesisCModel&fl=PID&rows=10000';
    $supp_solr_query = $solr_url . '/select/?q='.$supp_embargo_field.':[NOW/DAY-7DAY%20TO%20NOW/DAY]&fl=PID&rows=10000';
    $supp_solr_result = @file_get_contents($supp_solr_query);

    if ($supp_solr_result != NULL) {
      $xml = new SimpleXMLElement($supp_solr_result);
      $array = $xml->xpath('//doc');
      $array = array_filter($array);
      $pids = array();
      foreach ($array as $arr) {
        $pid = (string) $arr[0]->str;
        drupal_set_message(t('PID: @pid', array('@pid' => $pid)));
        $pids[] = $pid;
        $item = new Fedora_Item($pid);
        $ds_array = $item->get_datastreams_list_as_array();
        foreach ($ds_array as $key => $ds) {
          if ($key == 'POLICY') {
            $policy_details = $item->get_datastream_info('POLICY');
            drupal_set_message(t('Policy label: @label', array('@label' => $policy_details->datastream->label)));
            switch ($policy_details->datastream->label) {
              case 'POLICY':
              case 'Embargo policy - Both':
              $result_remove = $item->purge_datastream('POLICY');
              $result_add = $item->add_datastream_from_file($scholar_path . '/xacml/supplemental-embargo.xml', 'POLICY', 'Embargo policy - Doc');
              break;
              case 'Embargo policy - Supp';
              $result_remove = $item->purge_datastream('POLICY');
              break;
              case 'Embargo policy - Doc':
              drupal_set_message(t('Supplemental info embargo for object \'@pid\' already removed!', array('@pid' => $pid)), 'warning');
              break;
              default:
              drupal_set_message(t('Unknown policy on object \'@pid\', unable to remove!', array('@pid' => $pid)), 'warning');
            }
          }
        }
      }
    }
  }

  if (!empty ($embargo_field) && !empty ($supp_embargo_field)) {
    // Remove POLICY datastream from objects that don't have an embargo set for either the document or
    // the supplemental information.

    //$none_solr_query = $solr_url . '/select/?q=-'.$embargo_field.':[*%20TO%20*]%20OR%20-'.$supp_embargo_field.':[*%20TO%20*]%20AND%20rels.hasModel:thesis&fl=PID&rows=10000';
    $none_solr_query = $solr_url . '/select/?q=-'.$embargo_field.':[*%20TO%20*]%20OR%20-'.$supp_embargo_field.':[*%20TO%20*]&fl=PID&rows=10000';
    $none_solr_result = @file_get_contents($none_solr_query);

    if ($none_solr_result != NULL) {
      $xml = new SimpleXMLElement($none_solr_result);
      $array = $xml->xpath('//doc');
      $array = array_filter($array);
      $pids = array();
      foreach ($array as $arr) {
        $pid = (string) $arr[0]->str;
        drupal_set_message(t('PID: @pid', array('@pid' => $pid)));
        $pids[] = $pid;
        $item = new Fedora_Item($pid);
        $ds_array = $item->get_datastreams_list_as_array();
        foreach ($ds_array as $key => $ds) {
          if ($key == 'POLICY') {
            $result_remove = $item->purge_datastream('POLICY');
            if ($result_remove != NULL) {
              drupal_set_message(t('Embargo policy for object @pid removed!', array('@pid' => $pid)));
            }
            else {
              drupal_set_message(t('Unable to remove embargo policy for object @pid!', array('@pid' => $pid)));
            }
          }
        }
      }
    }
  }
}
