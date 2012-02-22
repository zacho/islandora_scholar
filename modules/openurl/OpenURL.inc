<?php

/**
 * @file
 *
 * Provides some convience functions for generating OpenURL links.
 */

/**
 * Given a base url and some parementers this function generates a OpenURL link.
 *
 * @param string $url
 *   The base resolver portion of a OpenURL
 * @param array $params
 *
 * @return string
 *   A OpenURL link.
 */
function openurl_link($url, array $params) {
  /**
   * @todo Implement based on documentation.
   */
  return '';
}

/**
 * Theme's a OpenURL link.
 *
 * @param string $url
 *   A OpenURL link,
 * @param string $icon
 *   A path to an option icon to display as the OpenURL link.
 *
 * @return string
 *   The html link.
 */
function theme_openurl_link($url, $icon = NULL) {
  return '<a href="/" title="Change Me!">Change Mes</a>';
}
