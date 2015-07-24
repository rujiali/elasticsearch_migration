<?php
/**
 * @file
 *
 */

/**
 * Menu callback for elasticsearch migration setting page.
 */
function elasticsearch_migration_form($form) {
  $form['elasticsearch_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Host'),
    '#description' => t('For example: http://localhost:9200'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['elasticsearch_cluster'] = array(
    '#type' => 'textfield',
    '#title' => t('Cluster'),
    '#description' => t('Name cluster name'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['elasticsearch_content_type'] = array(
    '#type' => 'textfield',
    '#title' => t('Content type'),
    '#description' => t('The content type you want to migrate'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Migrate'),
  );

  return $form;
}
