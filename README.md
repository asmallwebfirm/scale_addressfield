Scale Address Field
===================

A utility Drupal extension that helps Drupal scale when unauthenticated users
are presented with forms containing [Address Fields](). This extension requires
no configuration.


### Versions

Version | Branch | Build Status | Focus
------- | ------ | ----------- | -----
2.x     | 7.x-2.x | [![Build Status](https://travis-ci.org/asmallwebfirm/scale_addressfield.png?branch=7.x-2.x)](https://travis-ci.org/asmallwebfirm/scale_addressfield) | This branch focuses on website scalability; it's recommended that you test with your theme before deploying.
1.x     | 7.x-1.x | [![Build Status](https://travis-ci.org/asmallwebfirm/scale_addressfield.png?branch=7.x-1.x)](https://travis-ci.org/asmallwebfirm/scale_addressfield) | _Deprecated_. This branch focuses on UX bugs; updating to Drupal 7.27 resolves most bugs that this version addresses (see [the 1.x README]() for details).


### Reasons to use this module

- If you present forms containing Address Fields to unauthenticated users and
  these forms generate a large amount of traffic, this module resolves the
  following symptoms, with which you may be painfully familiar:
  - An extremely large cache_form table,
  - Stability issues caused by database locks during cache_form deletes during
    cron runs,
  - Unreasonably high disk IOPS on your database container
- Beyond scalability wins, you might want to use this module because:
  - UX: It makes country selection / field swapping instantaneous!
  - It introduces a sane way to override address field labels, select options,
    etc.


### Installation instructions

The best way to install this module is to use [drush][]! You can do so
using the following command:

```
drush dl scale_addressfield --source=http://www.asmallwebfirm.net/drupal/release-history
```

Enable the module like you would any other module, for example:

```
drush en scale_addressfield
```


### Configuration and overrides

Scale Address Field offers two ways in which you can alter or override the way
in which the address field dynamic country form widget is rendered.

#### Implementing hook_addressfield_config_pre_alter()

You can implement a simple alter to slightly modify the way in which fields are
rendered. See [scale_addressfield.api.php](scale_addressfield.api.php) for full
details.

```php
/**
 * Implements hook_addressfield_config_pre_alter().
 */
function MY_MODULE_addressfield_config_pre_alter(&$config) {
  // You might want to update the label for the country field itself.
  // Note: Do not run these through t(), they are translated via i18n
  $config['label'] = 'Country/Region';

  // For full details on structure, see the default address configuration file,
  // located at json/addressfield.json.
}
```

See also hook_addressfield_config_post_alter() for altering configuration
details after configuration localization and processing has occurred.

#### The `scale_addressfield_config_json` configuration

For more significant changes to address formats, you can provide an alternate
default address field configuration file. Although there's no UI, it's as simple
as setting a `$conf` setting in your settings.php file. For instance:

```php
$conf['scale_addressfield_config_json'] = DRUPAL_ROOT . '/sites/all/libraries/my-addressfield-config.json';
```

The contents of `my-addressfield-config.json` should match the format of
[config/address-formats.json](json/addressfield.json). For more details, see the
[addressfield.json](https://github.com/tableau-mkt/addressfield.json) project on
GitHub.

#### Client-side reactions

There may be cases in which you wish to further customize the address field UX
on the client-side, but wish to do so after jquery.addressfield and the configs
associated with Scale Address Field have been initialized.

To do so, you can hang your customizations off of a custom event, triggered
after all configurations have been loaded and applied:

```javascript
(function($) {
  $(document).bind('scale_addressfield:initialized', function() {
    // Perform dependent customizations here.
  });
})(jQuery);
```

#### Localization

This module utilizes i18n and i18n_string to localize end-user strings. You will
need to download and enable [i18n](). All user-facing strings are available for
translation under the "address field" group in the string translation UI.


### Contributing

Be mindful that this extension is built on top of other open source software! If
you find a bug in the integration of one of the following two projects with
Drupal, you're in the right place. File an issue or pull request to the right.
If you find an issue that relates specifically with one of the following, please
open up an issue or pull request against the project itself.

__jquery.addressfield__
- Used for client-side interaction. Report bugs and open pull requests
  associated with this behavior [here](https://github.com/tableau-mkt/jquery.addressfield).

__addressfield.json__
- Used as the source for default address configuration. If you think a country's
  address format is not represented correctly, open a pull request
  [here](https://github.com/tableau-mkt/addressfield.json).


[Address Fields]: https://drupal.org/project/addressfield
[the 1.x README]: https://github.com/asmallwebfirm/scale_addressfield/blob/7.x-1.x/README.md
[drush]: https://github.com/drush-ops/drush
[i18n]: https://drupal.org/project/i18n
