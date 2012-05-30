<div id="overview">
  <div id="citation">
    <h3 style="display:inline">
      <?php print t('Citation:') ?>
    </h3> 
    <select id="style" class="form-select" onchange="jQuery('.citeproc-bibliography').attr('id', this.value); Drupal.settings.citeproc.refresh = true; Drupal.attachBehaviors();">
      <?php foreach ($styles as $id => $name) : ?>
        <option value="<?php print $id ?>"><?php print $name ?></option>
      <?php endforeach; ?>
    </select>
    <?php print print_r($citation[0], TRUE); ?>
    <?php print drupal_get_form('islandora_bibliography_citation_form', $pid); ?>
  </div>
  <?php if (!empty($accessnum)): ?>
    <a id="open-url-link" class="overview-field" href="http://linksource.ebsco.com/linking.aspx?ID=12411?sid=Entrez:PubMed&amp;id=pmid:<?php print $accessnum; ?>" target="_blank"><img alt="Click here to read" id="linkout-icon-unknown-DB__1__13__4178" border="0" src="<?php print $open_url_img; ?>"></a>
  <?php endif; ?>
  <?php if (!empty($abstract)): ?>
    <div id="abstract" class="overview-field">
      <h3><?php print t('Abstract:') ?></h3>
      <div id="abstract-content" class="overview-field-content"><?php print print_r($abstract, TRUE); ?></div>
    </div>
  <?php endif; ?>
  <?php if (!empty($subject)): ?>
    <div id="subject" class="overview-field">
      <h3><?php print t('Subject Heading(s):') ?></h3>
      <div id="subject-content" class="overview-field-content"><?php print print_r($subject, TRUE); ?></div>
    </div>
  <?php endif; ?>
  <?php if (!empty($work_type)): ?>
    <div id="work-type" class="overview-field">
      <h3><?php print t('Work Type:') ?></h3>
      <div id="work-type-content" class="overview-field-content"><?php print print_r($work_type, TRUE); ?></div>
    </div>
  <?php endif; ?>
  <?php if (!empty($grant_number)): ?>
    <div id="grant-number" class="overview-field">
      <h3><?php print t('Grant Number(s):') ?></h3>
      <div id="grant-number-content" class="overview-field-content"><?php print print_r($grant_number, TRUE); ?></div>
    </div>
  <?php endif; ?>
  <?php if (!empty($funding_agency)): ?>
    <div id="funding-agency" class="overview-field">
      <h3><?php print t('Funding Agency(s):') ?></h3>
      <div id="funding-agency-content" class="overview-field-content"><?php print print_r($funding_agency, TRUE); ?></div>
    </div>
  <?php endif; ?>
  <?php if (!empty($core_facilties)): ?>
    <div id="core-facilties" class="overview-field">
      <h3><?php print t('Core facilties:') ?></h3>
      <div id="core-facilties-content" class="overview-field-content"><?php print print_r($core_facilties, TRUE); ?></div>
    </div>
  <?php endif; ?>
  <?php if (!empty($source_system)): ?>
    <div id="source-system" class="overview-field">
      <h3><?php print t('Source System:') ?></h3>
      <div id="source-system-content" class="overview-field-content"><?php print print_r($source_system, TRUE); ?></div>
    </div>
  <?php endif; ?>
  <?php if (!empty($ccsg_program)): ?>
    <div id="ccsg-program" class="overview-field">
      <h3><?php print t('CCSG Program:') ?></h3>
      <div id="ccsg-program" class="overview-field-content"><?php print print_r($ccsg_program, TRUE); ?></div>
    </div>
  <?php endif; ?>
  <?php if (!empty($peer_reviewed)): ?>
    <div id="peer-reviewed" class="overview-field">
      <h3><?php print t('Peer Reviewed:') ?></h3>
      <div id="peer-reviewed-content" class="overview-field-content"><?php print print_r($peer_reviewed, TRUE); ?></div>
    </div>
  <?php endif; ?>
  <?php if (!empty($url)):?>
    <div id="urls" class="overview-field">
      <h3><?php print t('Publisher URL:') ?></h3>
      <ul>
      <?php foreach (explode('<br/>', $url) as $a_url):?>
        <li class="overview-field-content"><?php print l($a_url, $a_url); ?></li>
      <?php endforeach; ?>
      </ul>
    </div>
  <?php endif; ?>
</div>
