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
  return $stats;
}
/**
 * Add datastream download links to the view object page.
 *
 * @param AbstractObject $object
 *   A Tuque FedoraObject.
 * @param array $ds
 *   An array of DSID's to provide a link for.
 * @param array $rendered
 *   The array of rendered views.
 *
 * @return string
 *   The rendered string, including added links.
 */
function metrodora_create_download_link($object, $ds, $rendered) {
  $print_title = FALSE;
  $link['dl_link_wrapper'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'download-link-wrapper',
      ),
    ),
  );
  $options = array(
    "attributes" => array(
      'title' => NULL,
    ),
  );
  foreach ($ds as $key => $dsid) {
    // Requested datastream may not be available.
    $access = islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object[$dsid]);
    if (isset($object[$dsid]) && $access) {
      $options['attributes']['title'] = t("Download this object's @dsid datastream", array("@dsid" => $dsid));
      $name = "ds_link_" . $dsid;
      $link['dl_link_wrapper'][$name] = array(
        '#markup' => l(t("Download"), "islandora/object/$object->id/datastream/$dsid/download", $options),
        '#suffix' => "</br>",
      );
      $print_title = TRUE;
    }
  }
  // Add the header if there are download links being output to the page.
  if ($print_title) {
    $link['dl_link_wrapper']['#prefix'] = '<div class="download-link-container"><h2>Downloads</h2>';
    $link['dl_link_wrapper']['#suffix'] = '</div>';
  }
  // Render the created container and links.
  $link_rendered = render($link);
  foreach ($rendered as $key => $value) {
    $rendered[$key]['#markup'] .= $link_rendered;
  }
  return $rendered;
}
