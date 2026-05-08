# 📋 Software Development Methodologies

> A comprehensive guide to the most widely used working methodologies in software and engineering projects — including advantages, disadvantages, industry adoption, and flexibility ratings.

---

## Table of Contents

1. [Waterfall](#1-waterfall)
2. [Scrum](#2-scrum)
3. [Kanban](#3-kanban)
4. [SAFe – Scaled Agile Framework](#4-safe--scaled-agile-framework)
5. [Lean](#5-lean)
6. [XP – Extreme Programming](#6-xp--extreme-programming)
7. [V-Model](#7-v-model)
8. [Spiral](#8-spiral)
9. [RAD – Rapid Application Development](#9-rad--rapid-application-development)
10. [Shape Up](#10-shape-up)
11. [Industry Adoption Summary](#-industry-adoption-summary)
12. [Flexibility Comparison Chart](#-flexibility-comparison-chart)

---

## 1. Waterfall

**Overview:**
A linear, sequential process where each phase (Requirements → Design → Development → Testing → Deployment → Maintenance) must be fully completed before the next one begins. No overlaps, no going back.

**Flexibility Level: 1 / 10**
> Nearly zero flexibility. Once a phase is signed off, changes are extremely costly and often require restarting from an earlier stage.

### ✅ Advantages
- Clear structure and well-defined milestones
- Easy to manage due to rigid phases
- Extensive documentation produced at every stage
- Ideal when requirements are fixed and well understood
- Predictable timelines and budgets

### ❌ Disadvantages
- No room for changes once a phase is complete
- Testing happens very late — bugs discovered at the end are expensive to fix
- Customer sees the product only at the very end
- High risk for long projects where requirements evolve

### 🏭 Industry Adoption
| Industry | Usage |
|---|---|
| Construction & Civil Engineering | ✅ Very common |
| Government / Public Sector | ✅ Standard |
| Defence (traditional contracts) | ✅ Common |
| Banking & Finance (legacy systems) | ✅ Common |
| Automotive (hardware specs) | ⚠️ Partial |

---

## 2. Scrum

**Overview:**
An Agile framework that organizes work into short, fixed-length iterations called **sprints** (2–4 weeks). Teams hold daily standups, sprint planning, reviews, and retrospectives. Roles: Product Owner, Scrum Master, Development Team.

**Flexibility Level: 8 / 10**
> High flexibility within sprints. Requirements can change between sprints. Some rigidity exists within a sprint itself.

### ✅ Advantages
- Rapid delivery of working software
- Customer feedback incorporated frequently
- Transparent progress through sprint reviews
- Teams are self-organizing and motivated
- Problems surfaced early through retrospectives

### ❌ Disadvantages
- Scope creep if the backlog is not properly managed
- Requires strong commitment from the entire team
- Hard to scale across large organizations without additional frameworks (e.g., SAFe)
- Sprint deadlines can cause pressure and rushed work
- Not ideal for projects with very fixed requirements

### 🏭 Industry Adoption
| Industry | Usage |
|---|---|
| Software / Tech Startups | ✅ Dominant |
| E-Commerce | ✅ Very common |
| Media & Entertainment | ✅ Common |
| Telecommunications | ✅ Common |
| Defence (software-focused teams) | ⚠️ Growing |

---

## 3. Kanban

**Overview:**
A visual workflow management system using a board (columns: To Do → In Progress → Done). Work flows continuously without fixed sprints. The key principle is **limiting Work In Progress (WIP)** to reduce bottlenecks and improve throughput.

**Flexibility Level: 9 / 10**
> Extremely flexible. No sprints, no fixed iterations. Work is added or reprioritized at any time.

### ✅ Advantages
- Highly visual — bottlenecks are immediately obvious
- No time-boxed sprints means tasks flow naturally
- Reduces multitasking via WIP limits
- Easy to adopt incrementally alongside existing processes
- Great for ongoing, maintenance, or support work

### ❌ Disadvantages
- Lack of fixed deadlines can lead to tasks drifting
- Less structure than Scrum — can feel chaotic without discipline
- Not ideal for large-scale project planning
- Measuring velocity or estimating delivery is harder
- Requires team discipline to respect WIP limits

### 🏭 Industry Adoption
| Industry | Usage |
|---|---|
| IT Operations & DevOps | ✅ Very common |
| Customer Support Teams | ✅ Common |
| Marketing Teams | ✅ Common |
| Manufacturing (origin) | ✅ Toyota-origin |
| Healthcare Operations | ⚠️ Growing |

---

## 4. SAFe – Scaled Agile Framework

**Overview:**
SAFe is Agile scaled for large enterprises. It synchronizes multiple Scrum teams across **Agile Release Trains (ARTs)**, coordinating work at three levels: Team, Program, and Portfolio. Used when hundreds of people need to collaborate on a single product.

**Flexibility Level: 6 / 10**
> Moderate flexibility. Agile principles are preserved at the team level, but additional governance and planning cadences reduce agility at the program/portfolio level.

### ✅ Advantages
- Enables Agile across hundreds or thousands of developers
- Aligns team work with business strategy and portfolio goals
- Provides governance and compliance structures
- Regular PI (Program Increment) Planning creates shared alignment
- Works well in regulated industries

### ❌ Disadvantages
- Very complex to implement — requires significant training
- Heavy ceremonies and meetings can slow teams down
- Can feel bureaucratic — some call it "Agile in name only"
- Expensive to roll out at scale
- Risk of losing the core spirit of Agile under all the process

### 🏭 Industry Adoption
| Industry | Usage |
|---|---|
| Defence & Aerospace (large programs) | ✅ Growing rapidly |
| Banking & Financial Services | ✅ Common |
| Telecommunications | ✅ Common |
| Healthcare IT | ✅ Common |
| Automotive (connected systems) | ⚠️ Emerging |

---

## 5. Lean

**Overview:**
Originating from Toyota's **Toyota Production System (TPS)**, Lean focuses on eliminating waste (*Muda*), delivering maximum value with minimum resources, and continuous improvement (*Kaizen*). It is more of a philosophy than a methodology, and it heavily influenced Agile and Kanban.

**Flexibility Level: 7 / 10**
> Lean is adaptable to many contexts. Its principles are flexible, but its discipline around waste elimination and value stream mapping creates structured thinking.

### ✅ Advantages
- Eliminates waste and inefficiency across the entire value stream
- Continuous improvement culture keeps teams evolving
- Focuses on delivering real customer value
- Reduces lead time from idea to delivery
- Empowers workers to identify and fix problems

### ❌ Disadvantages
- Cultural transformation required — hard to implement in traditional organizations
- Requires sustained management commitment
- "Eliminating waste" can be misinterpreted as cutting essential steps
- Measuring value and waste can be subjective
- Less prescriptive than Scrum — teams need strong internal discipline

### 🏭 Industry Adoption
| Industry | Usage |
|---|---|
| Automotive Manufacturing | ✅ Origin industry |
| Healthcare | ✅ Lean Healthcare widely adopted |
| Software Development | ✅ Lean Software principles |
| Logistics & Supply Chain | ✅ Very common |
| Aerospace Manufacturing | ✅ Common |

---

## 6. XP – Extreme Programming

**Overview:**
An Agile software development methodology that pushes engineering best practices to the extreme: **Test-Driven Development (TDD)**, **pair programming**, continuous integration, small releases, and heavy customer involvement. Ideal for teams that value code quality above all else.

**Flexibility Level: 7 / 10**
> Very flexible in terms of requirements (customer is always present), but rigid in its engineering practices (TDD, pair programming are non-negotiable).

### ✅ Advantages
- Very high code quality through TDD and pair programming
- Defects caught early — testing is first-class
- Continuous integration ensures stability
- Customer is directly involved — no surprises at delivery
- Small, frequent releases reduce risk

### ❌ Disadvantages
- Pair programming is resource-intensive (two devs per task)
- Requires experienced, disciplined developers
- Customer must be available full-time — often unrealistic
- Heavy focus on code can undervalue design and architecture
- Not suitable for large teams without additional coordination frameworks

### 🏭 Industry Adoption
| Industry | Usage |
|---|---|
| Software Consultancies | ✅ Common |
| Startups (quality-focused) | ✅ Common |
| FinTech | ✅ Common |
| Embedded / Safety-critical systems | ⚠️ Adapted forms |
| Defence (rare, emerging) | ⚠️ Rare |

---

## 7. V-Model

**Overview:**
An extension of Waterfall where every development phase has a **directly corresponding testing phase**, forming a "V" shape. Development goes down the left side (requirements → design → coding), and testing goes up the right side (unit test → integration test → acceptance test).

**Flexibility Level: 2 / 10**
> Very rigid, like Waterfall but with mandatory parallel test planning. Changes mid-project are expensive and disruptive.

### ✅ Advantages
- Testing is planned from the very beginning, not as an afterthought
- Very high traceability between requirements and tests
- Ideal for safety-critical systems where every requirement must be verified
- Clear structure with defined entry/exit criteria for each phase
- Reduces risk of late-stage failures in regulated environments

### ❌ Disadvantages
- Inflexible — any requirement changes cascade through both sides of the V
- No working software until late in the project
- Expensive when requirements are incomplete or change
- Extensive documentation overhead
- Not suitable for fast-paced or innovative projects

### 🏭 Industry Adoption
| Industry | Usage |
|---|---|
| Automotive (ISO 26262 functional safety) | ✅ Standard |
| Aerospace (DO-178C avionics) | ✅ Standard |
| Defence (MIL-STD, STANAG compliance) | ✅ Very common |
| Medical Devices (IEC 62304) | ✅ Standard |
| Railway Systems (EN 50128) | ✅ Standard |

---

## 8. Spiral

**Overview:**
Combines the structured phases of Waterfall with the iterative nature of Agile. Each "spiral" cycle goes through four quadrants: **Planning → Risk Analysis → Engineering → Evaluation**. Projects loop through these phases repeatedly, reducing risk at each cycle.

**Flexibility Level: 5 / 10**
> Moderate flexibility. The iterative nature allows adaptation, but the formal risk analysis and planning overhead keeps it more structured than Agile.

### ✅ Advantages
- Systematic risk management at every stage
- Good for large, complex, and high-risk projects
- Allows prototypes to be built and evaluated early
- Flexibility to accommodate changes between spirals
- Strong focus on customer evaluation throughout

### ❌ Disadvantages
- Complex to manage — requires experienced risk analysts
- Can be expensive due to repeated planning and risk assessment cycles
- End date can be unclear — spirals can continue indefinitely
- Overkill for small or low-risk projects
- Requires strong management discipline

### 🏭 Industry Adoption
| Industry | Usage |
|---|---|
| Large-scale Government IT projects | ✅ Common |
| Defence (R&D programs) | ✅ Common |
| Aerospace (new system development) | ✅ Common |
| Space exploration software | ✅ Used (e.g. NASA) |
| High-risk enterprise software | ⚠️ Occasional |

---

## 9. RAD – Rapid Application Development

**Overview:**
RAD prioritizes fast prototyping and early user feedback over extensive upfront planning and documentation. Users are involved from the start and prototypes are quickly refined. Suitable when speed to market is critical and requirements can be discovered through iteration.

**Flexibility Level: 8 / 10**
> Very flexible by design. The entire philosophy is built around accommodating change through rapid iterative prototyping.

### ✅ Advantages
- Extremely fast delivery of initial versions
- Heavy user involvement reduces the risk of building the wrong thing
- Easy to accommodate changing requirements
- Reusable components speed up development
- Customer gets a visible, working product very quickly

### ❌ Disadvantages
- Requires highly skilled developers capable of rapid iteration
- Poor documentation due to speed focus
- Not suitable for large teams or complex systems
- Technical debt accumulates if prototypes are pushed to production
- Requires constant customer availability

### 🏭 Industry Adoption
| Industry | Usage |
|---|---|
| Web & Mobile App Development | ✅ Very common |
| E-Commerce | ✅ Common |
| Startups / MVPs | ✅ Very common |
| Marketing / Internal Tooling | ✅ Common |
| Enterprise software (limited) | ⚠️ Rare |

---

## 10. Shape Up

**Overview:**
Developed by **Basecamp**, Shape Up replaces sprints with **6-week cycles**. Work is "shaped" (scoped and de-risked) by senior staff before a cycle begins. Teams have full autonomy during the cycle. There are 2-week cooldown periods between cycles. No daily standups, no backlogs.

**Flexibility Level: 6 / 10**
> Flexible within cycles (teams decide how to solve problems), but the shaping and betting process means commitment is made upfront per cycle.

### ✅ Advantages
- No sprint pressure or arbitrary velocity tracking
- Teams have genuine autonomy and ownership
- Appetite-based scoping prevents projects from bloating
- Cooldown periods allow reflection and cleanup
- Works very well for experienced, senior product teams

### ❌ Disadvantages
- Less well-known — adoption is limited outside product companies
- Shaping requires experienced senior staff
- Not ideal for large teams or highly regulated projects
- No formal backlog can feel chaotic for teams used to structure
- Hard to coordinate across multiple teams simultaneously

### 🏭 Industry Adoption
| Industry | Usage |
|---|---|
| Product Companies (SaaS) | ✅ Growing |
| Tech Startups | ✅ Growing |
| Digital Agencies | ⚠️ Occasional |
| Enterprise / Regulated Industries | ❌ Rare |

---

## 🏭 Industry Adoption Summary

| Industry | Primary Methodology | Secondary / Also Used |
|---|---|---|
| **Automotive** (hardware/safety) | V-Model, Waterfall | Lean, Agile (software) |
| **Aerospace** (avionics, DO-178C) | V-Model, Spiral | Waterfall, SAFe |
| **Defence** (mission-critical systems) | V-Model, Waterfall | SAFe, Scrum (software teams) |
| **Medical Devices** (IEC 62304) | V-Model, Waterfall | Lean |
| **Railway Systems** | V-Model | Waterfall |
| **Banking & Finance** | Waterfall (legacy) | SAFe, Scrum |
| **Telecommunications** | Scrum, SAFe | Kanban |
| **Software / Tech** | Scrum, Kanban | XP, Shape Up |
| **Startups / MVPs** | Scrum, RAD | Kanban, Shape Up |
| **Manufacturing** | Lean, Kanban | Waterfall |
| **Healthcare IT** | SAFe, Scrum | Lean, V-Model |
| **Government IT** | Waterfall, Spiral | SAFe |

---

## 📊 Flexibility Comparison Chart

```
Methodology         Flexibility (0–10)
─────────────────────────────────────────────────────────
Waterfall           ▓░░░░░░░░░  1/10   (Nearly none)
V-Model             ▓▓░░░░░░░░  2/10   (Very rigid)
Spiral              ▓▓▓▓▓░░░░░  5/10   (Moderate)
SAFe                ▓▓▓▓▓▓░░░░  6/10   (Structured Agile)
Shape Up            ▓▓▓▓▓▓░░░░  6/10   (Cycle-bound)
Lean                ▓▓▓▓▓▓▓░░░  7/10   (Philosophy-based)
XP                  ▓▓▓▓▓▓▓░░░  7/10   (Flexible reqs, rigid engineering)
Scrum               ▓▓▓▓▓▓▓▓░░  8/10   (High, sprint-bounded)
RAD                 ▓▓▓▓▓▓▓▓░░  8/10   (Very adaptive)
Kanban              ▓▓▓▓▓▓▓▓▓░  9/10   (Maximum flow flexibility)
─────────────────────────────────────────────────────────
0 = No flexibility (rigid, sequential)
10 = Full flexibility (change anytime, continuous flow)
```

---

## 📝 Key Takeaways

- **Safety-critical industries** (aerospace, automotive, defence, medical) overwhelmingly use **V-Model** or **Waterfall** because compliance, traceability, and certification requirements demand it.
- **Software-heavy and tech industries** favour **Scrum** and **Kanban** for their speed and adaptability.
- **Large enterprises** increasingly adopt **SAFe** to bring Agile principles into traditional organisations without abandoning governance.
- **Startups and product companies** gravitate toward **Kanban**, **RAD**, or **Shape Up** for maximum speed and autonomy.
- In practice, **hybrid approaches** are the norm — for example, V-Model for hardware development combined with Scrum for the accompanying software stack.

---

*Document version: 1.0 — May 2026*
