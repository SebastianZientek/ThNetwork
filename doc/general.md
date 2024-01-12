# Requirements

## Host
1. Accepts or rejects `PAIR MSG` from `Transmitter`.
2. Sends configuration to `Transmitters` (update interval).
3. Accepts `SENSOR DATA MSG` from `Transmitter`.
4. Collects data from `Transmitters`.
5. Provides `Webpage` with sensors data presentation.
6. Provides `Admin` webpage
7. Allows to rename sensor by `Admin` webpage
8. Requires credentials to login to `Admin` page
9. Allows to change credentials on `Admin` page
10. Allows removal of paired `Transmitters`.

## Transmitter
1. Sends `PAIR` message to pair with `Host`.
2. Sends `SENSOR DATA MSG` to `Host`
3. Gets configuration data from `Host` during pairing

# Detailed description

# Q&A
1. How to identify sensor in system?:
    - each sensor should have id calculated from mac addr (can be std::hash).
2. Where should be saved information about sensors and mapping between ID's and names?
    - ConfStorage reads/saves mapping between sensor name and macAddr, maybe it is the right place?
3. How to separate interfaces between classes, and make code testable?
    - where it is needed, change class to template and provide concrete implementation as parameter
