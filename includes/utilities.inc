<?php
/**
 * @file
 * Utility functions.
 */

/**
 * Renderable aray of usage stats for the PDF containing DS of the given object.
 */
function mlmlora_get_pdf_stats($object) {
  module_load_include('inc', 'islandora_usage_stats', 'includes/db');
  if (isset($object['PDF'])) {
    $target_dsid = 'PDF';
  }
  elseif (in_array('islandora:sp_pdf', $object->models)) {
    $target_dsid = 'OBJ';
  }
  else {
    return;
  }
  // Requested datastream may not be available.
  if (!islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object[$target_dsid])) {
    return;
  }
  $dses = islandora_usage_stats_get_datastream_downloads_count($object->id);
  // Datastream may not have been downloaded.
  if (!isset($dses[$target_dsid])) {
    return;
  }

  $stats = t("@download_count", array(
    '@download_count' => format_plural($dses[$target_dsid], 'Downloaded @count time.', 'Downloaded @count times.'),
  ));

  return $download_stats['stats_wrapper'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'download-stats-wrapper',
      ),
    ),
    "ds_stats_$target_dsid" => array(
      '#markup' => $stats,
    ),
  );
}
