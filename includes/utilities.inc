<?php

/**
 * @file
 * Utility functions file
 */

/**
 * An array of usage stats per datastream provided.
 *
 * @param AbstractObject $object
 *   A Tuque FedoraObject.
 * @param array $ds
 *   An array of DSID's to get usage stats for.
 *
 * @return array
 *   An array of usage stats, to be used as output for theme display.
 */
function metrodora_get_stats_details($object, $ds, $rendered) {
  module_load_include('inc', 'islandora_usage_stats', 'includes/db');
  $stats = array();
  // Object View count.
  $stats[] = t("@view_count", array(
      '@view_count' => format_plural(islandora_usage_stats_get_individual_object_view_count($object->id), 'Viewed @count time', 'Viewed @count times'),
  ));
  // Grab the download stats for the datastreams on the object.
  $dses = islandora_usage_stats_get_datastream_downloads_count($object->id);
  $mapping = array(
    'islandora:bookCModel' => array('PDF'),
    'islandora:pageCModel' => array('JPG'),
    'islandora:sp_basic_image' => array('MEDIUM_SIZE'),
    'islandora:sp_large_image_cmodel' => array('JPG'),
    'islandora:sp_document' => array('PDF'),
    'islandora:sp-audioCModel' => array('PROXY_MP3'),
    'islandora:sp_videoCModel' => array('MP4'),
    'ir:citationCModel' => array('PDF'),
    'ir:thesisCModel' => array('PDF'),
    'islandora:sp_pdf' => array('OBJ'),
  );
  $ds_map = array_intersect_key($mapping, drupal_map_assoc($object->models));
  $total_downloads = 0;
  foreach ($ds_map as $mapping) {
    foreach ($mapping as $dsid) {
      if (isset($dses[$dsid])) {
        $total_downloads += $dses[$dsid];
      }
    }
  }
  $stats[] = t("@download_count", array(
    '@download_count' => format_plural($total_downloads, 'Downloaded @count time', 'Downloaded @count times'),
  ));
  $print_title = FALSE;
  $download_stats['stats_wrapper'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'download-stats-wrapper',
      ),
    ),
  );
  $views = $stats[0];
  $downloads = $stats[1];
  $stats = $views."<br />".$downloads;

  foreach ($ds as $key => $dsid) {
    // Requested datastream may not be available.
    $access = islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object[$dsid]);
    if (isset($object[$dsid]) && $access) {
      $name = "ds_stats_" . $dsid;
      $download_stats['stats_wrapper'][$name] = array(
        '#markup' => $stats,
      );
      $print_title = TRUE;
    }
  }
    // Add the header if there are stats being output to the page.
  if ($print_title) {
    $download_stats['stats_wrapper']['#prefix'] = '<div class="download-stats-container"><h2>Stats</h2>';
    $download_stats['stats_wrapper']['#suffix'] = '</div>';
  }
  $stats_rendered = render($download_stats);
  foreach ($rendered as $key => $value) {
    $rendered[$key]['#markup'] .= $stats_rendered;
  }
  dsm($rendered);
  return $rendered;
}
