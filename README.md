Scale Address Field
=======================

This is a a utility Drupal extension that works around bugs in the 
[Address Field][] module and Drupal core that can cause frustrating
end-user bugs under medium-to-high user concurrency.

This extension requires no configuration. Just install it and it works.

This will become irrelevant when the following issues are addressed:

* <https://drupal.org/node/774876>
* <https://drupal.org/node/1861608>

### Installation instructions

The best way to install this module is to use [drush][]! You can do so
using the following command:

```
drush dl scale_addressfield --source=http://www.asmallwebfirm.net/drupal/release-history
```

[address field]: https://drupal.org/project/addressfield
[drush]: https://github.com/drush-ops/drush
