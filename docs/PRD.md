# Phase 5 PRD — UnitConverter

본 저장소의 **제품 요구사항 문서(PRD)** 요약 인덱스이다. 사용자 대면 계약·예시는 [README.md](../README.md)가 단일 출처이며, 본 문서는 Epic·Story·인수 추적용이다.

## 문서 구조

| PRD 절 | README 앵커 |
|--------|----------------|
| §1 개요·목표·비목표 | [개요](../README.md#개요-overview) |
| §2 사용자·시나리오 | [부록 Activities](../README.md#부록--6시간-activities-생성형-ai-활용) |
| §3 기능·입출력 계약 | [입력 형식](../README.md#입력-형식-계약), [출력 포맷](../README.md#출력-포맷) |
| §4 비기능·아키텍처 | [아키텍처](../README.md#아키텍처), [테스트](../README.md#테스트-실행) |
| §5 데이터 | [지원 단위](../README.md#지원-단위-및-비율), [설정 파일](../README.md#설정-파일-jsonyaml) |
| §6 출력 | [출력 포맷](../README.md#출력-포맷) |
| §7 성공 지표 | [기여 가이드](../README.md#기여-가이드-contributing), `.cursorrules` |

## Phase 4 추적

- **Epic:** SC-1~8 (커버리지, Gherkin, 레이어, 회귀)
- **Stories:** US-01~07
- **Gherkin:** 8 Scenarios + Background
- **기계 규칙:** [`.cursorrules`](../.cursorrules)

## 인수 게이트 (체크)

- [ ] Gherkin G-1~G-8 green
- [ ] `regression_minimum` 5/5 green
- [ ] entity line coverage ≥ 95%
- [ ] forbidden / layer review 0 violations

상세 매트릭스는 강의 Phase 4 Level 5 체크리스트를 따른다.
