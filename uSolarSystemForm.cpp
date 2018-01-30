//---------------------------------------------------------------------------

#include <fmx.h>
#include <cmath>
#pragma hdrstop

#include "uSolarSystemForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TSolarSystemForm *SolarSystemForm;

// �n���̌��]���a
const float TSolarSystemForm::RADIUS = 8.0;

// �n���̌��]�Ԋu�i�x�j
const float TSolarSystemForm::STEP_REVOLUTION_EARTH = 360.0f / 365.0f / 4.0f;
// �n���̎��]�Ԋu�i�x�j
const float TSolarSystemForm::STEP_ROTATION_EARTH = -360.0f / 48.0;
// ���z�̎��]�Ԋu�i�x�j
const float TSolarSystemForm::STEP_ROTATION_SUN = -360.f / 27.0 / 10.0;

// �n���̌��]�Ԋu�i1�N��40000�~���b�Ŏ����j
const int   TSolarSystemForm::INTERVAL_REVOLUTION_EARTH = (120000.0f / 365.0f / 4.0f  + 0.5);
// �n���̎��]�Ԋu�i30�������j
const int   TSolarSystemForm::INTERVAL_ROTATION_EARTH = (120000.0f / 365.0f / 48.0f + 0.5f);
// ���z�̎��]�Ԋu�i27���j
const int   TSolarSystemForm::INTERVAL_ROTATION_SUN = (120000.0f / 27.0 / 10.0 + 0.5f);

//---------------------------------------------------------------------------
__fastcall TSolarSystemForm::TSolarSystemForm(TComponent* Owner)
	: TForm(Owner),
	EarthPositionX(RADIUS),
	EarthPositionY(0.0),
	EarthRevolutionAngle(0.0),
	EarthRotationAngle(0.0),
    SunRotationAngle(0.0),
	Suspended(true),
	Terminated(false)
{
	// ���W�̏�����
	this->SphereEarth->Position->X = EarthPositionX;
	this->SphereEarth->Position->Z = EarthPositionY;

	// �n�������]������X���b�h��o�^�����s
	RevolveEarthThread = std::thread(TSolarSystemForm::RevolveEarth);
	// �n�������]������X���b�h��o�^�����s
	RotateEarthThread = std::thread(TSolarSystemForm::RotateEarth);
	// ���z�����]������X���b�h��o�^�����s
	RotateSunThread  = std::thread(TSolarSystemForm::RotateSun);
}
//---------------------------------------------------------------------------

void __fastcall TSolarSystemForm::Timer1Timer(TObject *Sender)
{
	// �e�I�u�W�F�N�g�̍��W�E��]�p��ݒ�
	this->SphereEarth->Position->X = EarthPositionX;
	this->SphereEarth->Position->Z = EarthPositionY;
	this->SphereEarth->RotationAngle->Y = EarthRotationAngle;
	this->SphereSun->RotationAngle->Y = SunRotationAngle;


    // �ĕ`��
	this->Invalidate();
}
//---------------------------------------------------------------------------

// �p�x�v�Z
void TSolarSystemForm::AddAngle(float& Angle, float diff)
{
	Angle += diff;
	if (Angle >= 360.0)  {
		Angle -= 360.0;
	} else if (Angle <= -360.0)  {
		Angle += 360.0;
	}
}
//---------------------------------------------------------------------------

void TSolarSystemForm::RevolveEarth()
{
	// �X���b�h�����s���ł���ԃ��[�v����
	while (!SolarSystemForm->Terminated)  {

		// �ꎞ��~�����H
		if (!SolarSystemForm->Suspended) {

			// �n���̕����p���v�Z
			AddAngle(SolarSystemForm->EarthRevolutionAngle, STEP_REVOLUTION_EARTH);

			float radian = SolarSystemForm->EarthRevolutionAngle * M_PI / 180.0;
			SolarSystemForm->EarthPositionX = TSolarSystemForm::RADIUS * std::cos(radian);
			SolarSystemForm->EarthPositionY = TSolarSystemForm::RADIUS * std::sin(radian);
		}

		// ���̌v�Z�^�C�~���O�҂�
		std::this_thread::sleep_for(std::chrono::milliseconds(INTERVAL_REVOLUTION_EARTH));
	}
}

//---------------------------------------------------------------------------
void TSolarSystemForm::RotateEarth()
{
	// �X���b�h�����s���ł���ԃ��[�v����
	while (!SolarSystemForm->Terminated)  {

		// �ꎞ��~�����H
		if (!SolarSystemForm->Suspended) {
			// ��]�p�v�Z
			AddAngle(SolarSystemForm->EarthRotationAngle, STEP_ROTATION_EARTH);
		}
		// ���̌v�Z�^�C�~���O�҂�
		std::this_thread::sleep_for(std::chrono::milliseconds(INTERVAL_ROTATION_EARTH));
	}
}

//---------------------------------------------------------------------------
void TSolarSystemForm::RotateSun()
{

	// �X���b�h�����s���ł���ԃ��[�v����
	while (!SolarSystemForm->Terminated)  {

		// �ꎞ��~�����H
		if (!SolarSystemForm->Suspended) {
			// ��]�p�v�Z
			AddAngle(SolarSystemForm->SunRotationAngle, STEP_ROTATION_SUN);
		}

		// ���̌v�Z�^�C�~���O�҂�
		std::this_thread::sleep_for(std::chrono::milliseconds(INTERVAL_ROTATION_SUN));
	}
}
//---------------------------------------------------------------------------

void __fastcall TSolarSystemForm::FormDestroy(TObject *Sender)
{
	this->Terminated = true;
	this->Suspended = true;

	// �X���b�h���~������
	RevolveEarthThread.detach();
	RotateEarthThread.detach();
	RotateSunThread.detach();
}
//---------------------------------------------------------------------------

void __fastcall TSolarSystemForm::SphereSunDblClick(TObject *Sender)
{
	if (Suspended) {
		this->Timer1->Enabled = true;
		this->Suspended = false;
	} else {
		this->Timer1->Enabled = false;
		this->Suspended = true;
	}
}

//---------------------------------------------------------------------------
void __fastcall TSolarSystemForm::ViewSpaceGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
		  bool &Handled)
{
	switch (EventInfo.GestureID) {
	case sgiScratchout: 	// ������ (�}�E�X�����E�ɓ�����)
			OnInit();
			break;
	case sgiLeft:       	// �}�E�X���������ɓ�����
			OnRotateY(10);
			break;
	case sgiRight:      	// �}�E�X���E�����ɓ�����
			OnRotateY(-10);
			break;
	case sgiUp:         	// �}�E�X��������ɓ�����
			OnRotateX(10);
			break;
	case sgiDown:       	// �}�E�X���������ɓ�����
			OnRotateX(-10);
			break;
	case sgiSemiCircleLeft: // �}�E�X�𔽎��v���ɉ�
			OnRotateZ(10);
			break;
	case sgiSemiCircleRight:// �}�E�X�����v���ɉ�
			OnRotateZ(-10);
			break;
	default:
		;
	}
}

//---------------------------------------------------------------------------

void __fastcall TSolarSystemForm::OnRotateX(int Direction)
{
	float a = this->SolarSystem->RotationAngle->X;
	this->SolarSystem->RotationAngle->X = a + Direction;
}

//---------------------------------------------------------------------------
void __fastcall TSolarSystemForm::OnRotateY(int Direction)
{
	float a = this->SolarSystem->RotationAngle->Y;
	this->SolarSystem->RotationAngle->Y = a + Direction;

}
//---------------------------------------------------------------------------

void __fastcall TSolarSystemForm::OnRotateZ(int Direction)
{
	float a = this->SolarSystem->RotationAngle->Z;
	this->SolarSystem->RotationAngle->Z = a + Direction;
}

//---------------------------------------------------------------------------
void __fastcall TSolarSystemForm::OnInit()
{
	this->SolarSystem->RotationAngle->X = 0.0;
	this->SolarSystem->RotationAngle->Y = 0.0;
	this->SolarSystem->RotationAngle->Z = 0.0;
}



