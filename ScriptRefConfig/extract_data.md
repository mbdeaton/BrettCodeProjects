# Perl: Parse Emails for Data
Brett Deaton - Feb 2023

`extract_data-??.pl`.

Extract data from a single text file concatenating text emails having a common
format like this:

```
From: someone@domain.com
Friday, February 24, 2023 6:20 AM
...

Daily Results

Station Usage   33%     Avg
Total Units     1692
Passing Units   1666    Good
Units with Failures/Aborts      100
Total Failures/Aborts   205
Avg Success Time        1.43
Avg Fail Time   0.81
Avg Idle Time   0.48    Avg
First Pass Yield        94%     Avg
Capacity Used   47%
Cap. Used on Failed Tests       8%
Daily Unit Goal         2500
Max. Potential Units    3560    Avg

Trend Data
...
```

The sequence of perl scripts represents my learning progression.