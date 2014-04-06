Scale Address Field [![Build Status](https://travis-ci.org/asmallwebfirm/scale_addressfield.png?branch=7.x-1.x)](https://travis-ci.org/asmallwebfirm/scale_addressfield)
=======================

This is a a utility Drupal extension that works around bugs in the 
[Address Field][] module and Drupal core that can cause frustrating
end-user bugs when address fields are presented to unauthenticated users.

This extension requires no configuration. Just install it and it works.

### Symptoms this extension resolves or mitigates

* Form validation errors for unrelated fields appearing when a country is
  selected,
* Erroneous validation errors when a user selects a country with a known list of
  administrative areas (potentially related to administrative area selections
  made by unrelated users in separate threads/requests),
* Other odd behavior related to users unknowingly affecting the state of a form
  for other users who may also be filling out the form simultaneously,
* Forms being unresponsive to country change (no dynamic administrative area or
  postal code behavior) near or around cron runs / form cache clears, or in
  cases where users have left a page open for extended periods of time.

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

### Optional configurations

This module, by default, will attempt to fix forms that have become "stale" due
to their rendered markup living on (in Drupal page cache, Varnish, etc) longer
than their associated cache_form entries. The mechanism for this depends on an
AJAX callback performing a "staleness" check at a configurable interval. There
is no UI for this configuration, but you can use drush to change its value; by
default, for scalability reasons, it's set to check once on initial render and
then every 5 minutes following. You can tune this value based on your available
resources, the rate at which this problem occurs, etc.

```
# Set the staleness check to run every 2 minutes (120 seconds).
drush vset scale_addressfield_ping_interval 120
```

```
# Or disable this functionality completely by setting the interval to 0.
drush vset scale_addressfield_ping_interval 0
```

### Relevant drupal.org issues

* <https://drupal.org/node/774876>
* <https://drupal.org/node/1861608>

[address field]: https://drupal.org/project/addressfield
[drush]: https://github.com/drush-ops/drush
