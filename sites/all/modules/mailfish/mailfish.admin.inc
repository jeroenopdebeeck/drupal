<?php
/**
 * @file
 * Provide the admin related functions for the MailFish module.
 */


/*
 * Menu callback.
 *
 * Displays mailfish signups.
 */
function mailfish_signups() {
    $output ='';
    $rows = array();
    $header = array(
        'User',
        'Node',
        'Email',
        'Created'
    );

    $fields = drupal_get_schema('mailfish');
    $query = db_select('mailfish', 'm');

    $table_alias = $query->innerJoin('node', 'n' , 'n.nid = m.nid', array());

    $results = $query->fields('m', array_keys($fields['fields']))
        ->fields($table_alias, array('title'))

        ->orderBy('m.created', $direction = 'ASC')

        ->execute()

        ->fetchAll();

    foreach ($results as $value) {
        $account = $value->uid ? user_load($value->uid) : '';
        $rows[] = array(
          $value->uid ? theme('username', array('account' => $account)) : '',
          $value->nid ? l($value->title, 'node/' . $value->nid) : '',
          $value->mail,
          date('F j, Y g:i A', $value->created),
        );
    }

    $output .= theme('table' , array('header' => $header, 'rows' => $rows));
    var_dump($output);
    return $output;
}




function mailfish_admin_settings_callback() {
    return drupal_get_form('mailfish_admin_settings_form');
}

function mailfish_admin_settings_form() {
    $form = array();
    $form['mailfish_types'] = array(
        '#title' => t('The content types to enable MailFish subscriptions for'),
        '#description' => t('On the specified node types, a MailFish subscription option will be available'),
        '#type' => 'checkboxes',
        '#options' => node_type_get_names(),
        '#default value' => variable_get('mailfish_types', array()),
    );
    return system_settings_form($form);
}