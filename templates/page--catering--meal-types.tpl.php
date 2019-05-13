<?php

/**
 * @file
 * Template file for 'catering_meal_choices' theme.
 *
 * Available variables:
 * - $meat: Total number of meat meal types selected.
 * - $fish: Total number of fish meal types selected.
 * - $veg: Total number of vegetarian meal types selected.
 */

?>

<div>
<table>
  <thead>
    <tr>
      <th>Meal Type</th>
  		<th>Total Meals</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Fish</td>
      <td><?php print $fish; ?></td>
    </tr>
    <tr>
      <td>Meat</td>
      <td><?php print $meat; ?></td>
    </tr>
    <tr>
      <td>Vegetarian</td>
      <td><?php print $veg; ?></td>
    </tr>
    <tbody>
	</table>
</div>