# Securing an Embedded System

This project focuses on analyzing and securing an embedded system architecture consisting of multiple devices connected to a central server. The system enables data collection and remote administrative control, including configuration updates and software management.

## Getting Started

Instructions for how to get a copy of the project running on your local machine.

```bash
git clone https://github.com/onuroezcelik/securing_embedded_system.git
```

### Dependencies

No external dependencies are required for this project.

### Installation

Step by step explanation of how to get a dev environment running.

1. Clone the repository:

```bash
git clone https://github.com/onuroezcelik/securing_embedded_system.git
```

## Testing

Explain the steps needed to run any automated tests

### Break Down Tests

Explain what each test does and why

```
Examples here
```
## Project Instructions

### STEP 1

### Simplified Threat Model
#### Scope

This threat model analyzes the architecture containing multiple embedded devices connected to a server.
The devices collect data and listen for server connections for two purposes:

- data collection
- administrative actions

Administrative actions include:

- changing data collection configuration
- updating device software
- disabling the device

#### Identified Assets

The following assets were identified as relevant for the simplified threat model:

- Collected data
Data collected by the embedded devices and transmitted to the server.
- Device firmware / software
The software running on the device, including update capability.
- Administrative control interface
The management communication path used by the server to configure, update, or disable devices.

STRIDE Analysis
Asset 1: Collected Data

| STRIDE Category            | Threat                                                                                    | Mitigation |
| -------------------------- | ----------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------- |
| **Spoofing**               | An attacker impersonates a legitimate device and sends fake collected data to the server. | Use mutual authentication between device and server with device certificates or unique credentials. |
| **Tampering**              | Data is modified in transit before reaching the server.                                   | Use encrypted and integrity-protected communication such as TLS and message authentication codes.   |
| **Repudiation**            | A device or attacker denies having sent specific data.                                    | Use signed logs, timestamps, and device identity in audit records.                                  |
| **Information Disclosure** | Sensitive collected data is intercepted during transmission or exposed at rest.           | Encrypt data in transit and at rest, and apply strict access control on the server.                 |
| **Denial of Service**      | Attackers flood the server or device communication path, preventing data delivery.        | Apply rate limiting, input validation, and retry/backoff mechanisms. |
| **Elevation of Privilege** | N/A                                                                                       | N/A |

Asset 2: Device Firmware / Software
| STRIDE Category            | Threat                                                                                         | Mitigation                                                                                   |
| -------------------------- | ---------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------- |
| **Spoofing**               | An attacker pretends to be the trusted update server and delivers a malicious firmware update. | Authenticate the server and use signed firmware images.                                      |
| **Tampering**              | Firmware is modified before or during installation.                                            | Verify firmware integrity with digital signatures and secure boot.                           |
| **Repudiation**            | There is no reliable evidence showing which software version was installed and when.           | Maintain secure audit logs of update events, versions, and timestamps.                       |
| **Information Disclosure** | Proprietary firmware or configuration details are extracted from the device.                   | Protect debug interfaces, encrypt sensitive storage, and restrict access to firmware images. |
| **Denial of Service**      | A corrupted or malicious update makes the device unavailable or non-functional.                | Use rollback protection, A/B partitioning, and update validation before activation.          |
| **Elevation of Privilege** | An attacker exploits a software flaw to gain privileged execution on the device.               | Apply least privilege, harden services, validate inputs, and keep software patched.          |

Asset 3: Administrative Control Interface
| STRIDE Category            | Threat                                                                                              | Mitigation                                                                                           |
| -------------------------- | --------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------- |
| **Spoofing**               | An attacker impersonates the server and sends unauthorized admin commands to a device.              | Use mutual authentication and strong credential management.                                          |
| **Tampering**              | Admin commands such as configuration changes or disable commands are altered in transit.            | Protect the channel with TLS and integrity checks.                                                   |
| **Repudiation**            | An administrator denies sending a configuration change, software update, or disable command.        | Use authenticated command logging with user identity, timestamps, and tamper-resistant audit trails. |
| **Information Disclosure** | Administrative commands or credentials are exposed to unauthorized parties.                         | Encrypt admin traffic and store credentials securely using secrets management.                       |
| **Denial of Service**      | Attackers block or overwhelm the admin interface so legitimate control actions cannot be performed. | Use network filtering, rate limiting, segmentation, and monitoring.                                  |
| **Elevation of Privilege** | A low-privileged user gains access to privileged device management functions.                       | Enforce role-based access control and least privilege for administrative actions.                    |

##### Summary of Main Risks

The main risks in this architecture are:

unauthorized devices sending false data
interception or modification of collected data
malicious or unauthorized firmware updates
abuse of the administrative interface to reconfigure, disable, or compromise devices

#### Recommended Mitigations

The most important mitigations for this system are:

Mutual authentication between devices and server
Encrypted communication for both data and administrative channels
Integrity protection for messages and firmware
Signed firmware updates and secure boot
Audit logging for data transfer and administrative actions
Role-based access control for admin operations
Availability protections such as rate limiting and monitoring

### Step 2

### Step 3

### Step 4

### Step 5

### Step 6

### Step 7

## Built With

* [Item1](www.item1.com) - Description of item
* [Item2](www.item2.com) - Description of item
* [Item3](www.item3.com) - Description of item

Include all items used to build project.

## License
[License](../LICENSE.md)
