<?php
/**
 * @file
 * The batch API file.
 */

/**
 * Processing batch.
 */
function batch_elasticsearch_process($host, $cluster, $type, $nids, &$context) {
  // Connect to elasticsearch host.
  $params['hosts'] = array($host);
  $client = new Elasticsearch\Client($params);
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_node'] = 0;
    $context['sandbox']['max'] = count($nids);
  }
  // Get all the node from the selected content type.
  $limit = 10;

  $query = new EntityFieldQuery();

  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'article')
    ->propertyCondition('status', NODE_PUBLISHED)
    ->propertyCondition('nid', $context['sandbox']['current_node'], '>')
    ->range(0, $limit);

  $result = $query->execute();

  if (isset($result['node'])) {
    $nids = array_keys($result['node']);
  }
  foreach ($nids as $nid) {
    $node = node_load($nid);
    $body = get_object_vars($node);
    $params['index'] = $cluster;
    $params['type'] = $type;
    $params['body'][] = array(
      'index' => array(
        '_id' => $body['nid'],
      )
    );
    unset($body['nid']);
    $params['body'][] = $body;

    $context['results'][] = check_plain($node->title);

    // Update our progress information.
    $context['sandbox']['progress']++;
    $context['sandbox']['current_node'] = $nid;
    $context['message'] = t('Now processing %node', array('%node' => $node->title));
  }
  unset($params['hosts']);
  $response = $client->bulk($params);
  if ($response['errors'] == TRUE) {
    $context['finish'] = 1;
  }

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Showing message after batch finished.
 */
function batch_elasticsearch_finished($success, $results, $operations) {
  if ($success) {
    // Here we do something meaningful with the results.
    $message = t('@count items successfully processed:', array('@count' => count($results)));
    $message .= theme('item_list', array('items' => $results));
    drupal_set_message($message);
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array('%error_operation' => $error_operation[0], '@arguments' => print_r($error_operation[1], TRUE)));
    drupal_set_message($message, 'error');
  }
}
