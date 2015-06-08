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
function mlmlora_get_stats_details($object, $ds) {
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
    'islandora:sp_document' => array('PDF'),
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
  return $stats;
}
