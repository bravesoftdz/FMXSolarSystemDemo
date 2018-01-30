//---------------------------------------------------------------------------

#ifndef uSolarSystemFormH
#define uSolarSystemFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls3D.hpp>
#include <FMX.MaterialSources.hpp>
#include <FMX.Objects3D.hpp>
#include <FMX.Types.hpp>
#include <FMX.Viewport3D.hpp>
#include <System.Math.Vectors.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Gestures.hpp>
#include <thread>
#include <atomic>

//---------------------------------------------------------------------------
class TSolarSystemForm : public TForm
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TViewport3D *ViewSpace;
	TTextureMaterialSource *TextureSun;
	TImage *ImageSpace;
	TSphere *SphereSun;
	TSphere *SphereEarth;
	TTextureMaterialSource *TextureEarth;
	TTimer *Timer1;
	TGestureManager *GestureManager1;
	TDummy *SolarSystem;
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall SphereSunDblClick(TObject *Sender);
	void __fastcall ViewSpaceGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled);

private:	// ���[�U�[�錾
	std::thread RevolveEarthThread; // �n�������]������X���b�h
	std::thread RotateEarthThread;  // �n�������]������X���b�h
	std::thread RotateSunThread;  	// ���z�����]������X���b�h

	bool  Terminated;         		// �X���b�h���I�����Ă��邩�H
	bool  Suspended;          		// �ꎞ��~�����H
	float EarthRevolutionAngle;   	// ���z�ƒn���̕����p�i�x�j
	float EarthPositionX;           // �n���̍��W
	float EarthPositionY;           // �n���̍��W
	float EarthRotationAngle;       // �n���̉�]�p
	float SunRotationAngle;         // ���z�̉�]�p

	void __fastcall OnInit();
	void __fastcall OnRotateX(int Direction);
	void __fastcall OnRotateY(int Direction);
	void __fastcall OnRotateZ(int Direction);

public:		// ���[�U�[�錾
	__fastcall TSolarSystemForm(TComponent* Owner);

private:
    static const float RADIUS;                      // ���]���a

	static const float STEP_REVOLUTION_EARTH;     	// �n���̌��]�Ԋu�i�x�j
	static const float STEP_ROTATION_EARTH;       	// �n���̎��]�Ԋu�i�x�j
	static const float STEP_ROTATION_SUN;         	// ���z�̎��]�Ԋu�i�x�j

	static const int   INTERVAL_REVOLUTION_EARTH; 	// �n���̌��]�Ԋu�i�~���b�j
	static const int   INTERVAL_ROTATION_EARTH;   	// �n���̎��]�Ԋu�i�~���b�j
	static const int   INTERVAL_ROTATION_SUN;     	// ���z�̎��]�Ԋu�i�~���b�j

	static void RevolveEarth(); 					// �n�������]������
	static void RotateEarth();  					// �n�������]������
	static void RotateSun();  						// ���z�����]������
	static void AddAngle(float& Angle, float diff); // �p�x�v�Z
};
//---------------------------------------------------------------------------
extern PACKAGE TSolarSystemForm *SolarSystemForm;
//---------------------------------------------------------------------------
#endif
